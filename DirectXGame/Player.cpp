#define NOMINMAX
#include "Player.h"
#include "MapChipField.h"
#include <algorithm>
#include <numbers>
#include <cassert>

using namespace KamataEngine;

void Player::Initialize(Model* model, Model* swordModel, Camera* camera, const Vector3& position) { // NULLポインタチェック
	assert(model);
	assert(swordModel);
	assert(camera);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	modelSword_ = swordModel;
	camera_ = camera;

	swordScale_ = Vector3(2.0f, 2.0f, 2.0f);

	// ワールド変換の初期化
	worldTransform_.Initialize();
	swordWorldTransform_.Initialize();
	worldTransform_.translation_ = position;
	swordWorldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	swordWorldTransform_.scale_ = swordScale_;
}

void Player::Update() {

	// ゲーム開始演出中は操作・物理処理を停止する
	if (!isControllable_) {
		velocity_ = {};
		isAttacking_ = false;

		// ワールド変換を更新
		WorldTransformUpdate(worldTransform_);
		WorldTransformUpdate(swordWorldTransform_);
		return;
	}

	if (damageCoolTime_ > 0) {
		damageCoolTime_--;
	}

	// 近接攻撃処理
	ProcessAttack();

	float swordOffsetX = 1.3f; // お好みの距離に調整してください
	swordWorldTransform_.translation_ = worldTransform_.translation_ + Vector3((lrDirection_ == LRDirection::kRight ? 1 : -1) * swordOffsetX, 0.0f, 0.0f);

	// ジャンプ入力をバッファに保存
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		jumpBufferTimer_ = kJumpBufferFrame;
	}

	if (jumpBufferTimer_ > 0) {
		jumpBufferTimer_--;
	}

	InputMove();

	// 衝突判定を初期化
	CollisionMapInfo collisionMapInfo;
	// 移動量に速度の値をコピー
	collisionMapInfo.move = velocity_;

	// マップ衝突チェック
	CheckMapCollision(collisionMapInfo);

	worldTransform_.translation_ += collisionMapInfo.move;

	// 天井に当たった?
	if (collisionMapInfo.ceiling) {
		velocity_.y = 0;
	}

	UpdateOnGround(collisionMapInfo);
	UpdateOnWall(collisionMapInfo);

	ProcessJump();

	// 旋回制御
	if (turnTimer_ > 0.0f) {

		turnTimer_ = std::max(turnTimer_ - (1.0f / 60.0f), 0.0f);

		// 左右の自キャラ角度テーブル
		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 自キャラの角度を設定する
		worldTransform_.rotation_.y = EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
	}

	// 行列を定数バッファに転送
	WorldTransformUpdate(worldTransform_);
	WorldTransformUpdate(swordWorldTransform_);
}

void Player::Draw() {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, *camera_);

	modelSword_->Draw(swordWorldTransform_, *camera_);
}

void Player::InputMove() {

	if (!onGround_) {
		ApplyGravity();
	}
	InputHorizontal();
}

Vector3 Player::GetWorldPosition() const {

	// ワールド座標を入れる変数
	Vector3 worldPos{};
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB Player::GetAABB() const {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb{};

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void Player::OnCollision(const Enemy* enemy) {
	(void)enemy;
	// ダメージ
	TakeDamage(20);
}

void Player::InputHorizontal() {

	// 左右入力がある？
	bool pushD = Input::GetInstance()->PushKey(DIK_D);
	bool pushA = Input::GetInstance()->PushKey(DIK_A);

	if (!(pushD || pushA)) {
		velocity_.x *= (1.0f - kAttenuation);
		return;
	}

	// 向き変更
	if (pushD && lrDirection_ != LRDirection::kRight) {
		lrDirection_ = LRDirection::kRight;
		turnFirstRotationY_ = worldTransform_.rotation_.y;
		turnTimer_ = kTimeTurn;
	} else if (pushA && lrDirection_ != LRDirection::kLeft) {
		lrDirection_ = LRDirection::kLeft;
		turnFirstRotationY_ = worldTransform_.rotation_.y;
		turnTimer_ = kTimeTurn;
	}

	// 加速
	float accel = kAcceleration;

	if (pushD) {
		velocity_.x += accel;
	} else if (pushA) {
		velocity_.x -= accel;
	}

	// 最大速度制限
	velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
}

void Player::ApplyGravity() {

	// 空中のみ重力
	if (onGround_) {
		return;
	}

	// 落下速度
	velocity_ += Vector3(0, -kGravityAcceleration, 0);

	// 2段ジャンプ中は落下を少し抑える
	if (velocity_.y < 0.0f && jumpCount_ == 2) {
		velocity_.y *= 0.98f;
	}

	// 落下速度制限
	velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
}

void Player::ProcessJump() {

	if (!Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		return;
	}

	// バッファがなければジャンプしない
	if (jumpBufferTimer_ <= 0) {
		return;
	}

	if (jumpCount_ >= kMaxJumpCount) {
		return;
	}

	// ジャンプ
	if (jumpCount_ == 0) {
		velocity_.y = kJumpAcceleration;
	} else {
		velocity_.y = kJumpAcceleration * 1.0f;
	}
	onGround_ = false;
	jumpCount_++;
	jumpBufferTimer_ = 0; // 消費
}

void Player::ProcessAttack() {

	if (!isAttacking_ && Input::GetInstance()->TriggerKey(DIK_J)) {
		isAttacking_ = true;
		attackTimer_ = kAttackFrame;

		// 剣振りの初期位置・角度を保存
		swordStartRotation_ = swordWorldTransform_.rotation_;
		swordStartTranslation_ = swordWorldTransform_.translation_;
	}

	if (!isAttacking_)
		return;

	attackTimer_--;

	float t = 1.0f - static_cast<float>(attackTimer_) / kAttackFrame;
	float dir = (lrDirection_ == LRDirection::kRight) ? 1.0f : -1.0f;

	if (t < 0.5f) {
		// 剣を後ろに引く
		float rate = t / 0.5f;
		swordWorldTransform_.rotation_.z = EaseOut(0.0f, -1.0f * dir, rate); // 剣をZ軸で回すイメージ
	} else {
		// 剣を振り下ろす
		float rate = (t - 0.5f) / 0.5f;
		swordWorldTransform_.rotation_.z = EaseIn(-1.0f * dir, 1.0f * dir, rate);
	}

	if (attackTimer_ <= 0) {
		isAttacking_ = false;
		swordWorldTransform_.rotation_.z = 0.0f; // 元に戻す
	}
}

AABB Player::GetAttackAABB() const {

	Vector3 pos = GetWorldPosition();

	// 向きによって前方に出す
	float dir = (lrDirection_ == LRDirection::kRight) ? 1.0f : -1.0f;

	Vector3 center{};
	center.x = pos.x + dir * kAttackOffset;
	center.y = pos.y;
	center.z = pos.z;

	AABB aabb{};
	aabb.min = {center.x - kAttackWidth / 2.0f, center.y - kAttackHeight / 2.0f, center.z - kAttackWidth / 2.0f};

	aabb.max = {center.x + kAttackWidth / 2.0f, center.y + kAttackHeight / 2.0f, center.z + kAttackWidth / 2.0f};

	return aabb;
}

void Player::CheckMapCollision(CollisionMapInfo& info) {
	CheckMapCollisionUp(info);
	CheckMapCollisionDown(info);
	CheckMapCollisionRight(info);
	CheckMapCollisionLeft(info);
}

void Player::CheckMapCollisionUp(CollisionMapInfo& info) {

	// 上昇あり？
	if (info.move.y <= 0) {
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew{};

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	// 真上の当たり判定を行う
	bool hit = false;
	// 左上点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex + 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	// 右上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex + 1);
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, +kHeight / 2.0f, 0));

		// 現在座標が壁の外か判定
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, +kHeight / 2.0f, 0));

		if (indexSetNow.yIndex != indexSet.yIndex) {
			// めり込み先ブロックの範囲矩形
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.move.y = std::max(0.0f, rect.bottom - worldTransform_.translation_.y - (kHeight / 2.0f + kBlank));
			// 天井に当たったことを記録する
			info.ceiling = true;
		}
	}
}

void Player::CheckMapCollisionDown(CollisionMapInfo& info) {

	// 下降あり？
	if (info.move.y >= 0) {
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew{};

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	// 真上の当たり判定を行う
	bool hit = false;
	// 左下点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex - 1);
	// 隣接セルがともにブロックであればヒット
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	// 右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex - 1);
	// 隣接セルがともにブロックであればヒット
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(0, -kHeight / 2.0f, 0));

		// 現在座標が壁の外か判定
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, -kHeight / 2.0f, 0));

		if (indexSetNow.yIndex != indexSet.yIndex) {
			//  めり込み先ブロックの範囲矩形
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.move.y = std::min(0.0f, rect.top - worldTransform_.translation_.y + (kHeight / 2.0f + kBlank));
			// 天井に当たったことを記録する
			info.landing = true;
		}
	}
}

void Player::CheckMapCollisionRight(CollisionMapInfo& info) {

	// 右移動あり？
	if (info.move.x <= 0) {
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew{};

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	// 真上の当たり判定を行う
	bool hit = false;

	// 右上点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex - 1, indexSet.yIndex);
	// 隣接セルがともにブロックであればヒット
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	// 右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex - 1, indexSet.yIndex);
	// 隣接セルがともにブロックであればヒット
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(+kWidth / 2.0f, 0, 0));

		// 現在座標が壁の外か判定
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(+kWidth / 2.0f, 0, 0));

		if (indexSetNow.xIndex != indexSet.xIndex) {
			// めり込み先ブロックの範囲矩形
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.move.x = std::max(0.0f, rect.left - worldTransform_.translation_.x - (kWidth / 2.0f + kBlank));
			// 壁に当たったことを判定結果に記録する
			info.hitWall = true;
		}
	}
}

void Player::CheckMapCollisionLeft(CollisionMapInfo& info) {

	// 右移動あり？
	if (info.move.x >= 0) {
		return;
	}

	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew{};

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	MapChipType mapChipTypeNext;
	// 真上の当たり判定を行う
	bool hit = false;

	// 左上点の判定
	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex + 1, indexSet.yIndex);
	// 隣接セルがともにブロックであればヒット
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	// 左下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	mapChipTypeNext = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex + 1, indexSet.yIndex);
	// 隣接セルがともにブロックであればヒット
	if (mapChipType == MapChipType::kBlock && mapChipTypeNext != MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + info.move + Vector3(-kWidth / 2.0f, 0, 0));

		// 現在座標が壁の外か判定
		MapChipField::IndexSet indexSetNow;
		indexSetNow = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(-kWidth / 2.0f, 0, 0));

		if (indexSetNow.xIndex != indexSet.xIndex) {
			// めり込み先ブロックの範囲矩形
			MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.move.x = std::max(0.0f, rect.right - worldTransform_.translation_.x - (kWidth / 2.0f + kBlank));
			// 壁に当たったことを判定結果に記録する
			info.hitWall = true;
		}
	}
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {

	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0}, // kRightBottom
	    {-kWidth / 2.0f, -kHeight / 2.0f, 0}, // kLeftBottom
	    {+kWidth / 2.0f, +kHeight / 2.0f, 0}, // kRightTop
	    {-kWidth / 2.0f, +kHeight / 2.0f, 0}  // kLeftTop
	};

	return center + offsetTable[static_cast<uint32_t>(corner)];
}

void Player::UpdateOnGround(const CollisionMapInfo& info) {

	if (onGround_) {

		if (velocity_.y > 0.0f) {
			onGround_ = false;
		} else {

			// 移動後の4つの角の座標
			std::array<Vector3, kNumCorner> positionsNew{};

			for (uint32_t i = 0; i < positionsNew.size(); ++i) {
				positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.move, static_cast<Corner>(i));
			}

			MapChipType mapChipType;
			// 真下の当たり判定を行う
			bool hit = false;
			// 左下点の判定
			MapChipField::IndexSet indexSet;
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom] + Vector3(0, kGroundSearchHeight, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}

			// 右下点の判定
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom] + Vector3(0, kGroundSearchHeight, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hit = true;
			}

			// 落下なら空中状態に切り替え
			if (!hit) {
				// 空中状態に切り替える
				onGround_ = false;
			}
		}

	} else {

		// 着地フラグ
		if (info.landing && velocity_.y <= 0.0f) {
			// 着地状態に切り替える（落下を止める）
			onGround_ = true;
			// 着地時にX速度を減衰
			velocity_.x *= (1.0f - kAttenuationLanding);
			// Y速度をゼロにする
			velocity_.y = 0.0f;
			jumpCount_ = 0;
		}
	}
}

void Player::UpdateOnWall(const CollisionMapInfo& info) {

	// 壁接触による減速
	if (info.hitWall) {
		velocity_.x *= (1.0f - kAttenuationWall);
	}

	if (info.hitWall && velocity_.y < 0.0f) {
		velocity_.y *= 0.9f;
	}
}

AABB Player::GetSwordAABB() const {
	Vector3 pos = swordWorldTransform_.translation_;
	Vector3 scale = swordWorldTransform_.scale_;
	float angleZ = swordWorldTransform_.rotation_.z;
	float dir = (lrDirection_ == LRDirection::kRight) ? 1.0f : -1.0f;

	// 剣の長さ（ローカル単位）をスケールで拡大縮小
	float swordLength = 1.0f * scale.x;
	float swordWidth = 0.3f * scale.y;
	float swordHeight = 0.8f * scale.z;
	float swordDepth = 0.15f * scale.z; // 深さはz方向のスケールで調整

	// 剣の向きベクトル（2D的にX軸方向に向く）
	Vector3 forward = Vector3(dir * std::cos(angleZ), 0.0f, 0.0f);

	// 剣の中心を少し手前にずらす（剣先すぎると当たらないので）
	Vector3 center = pos + forward * (swordLength * 0.25f);

	AABB aabb{};
	aabb.min = {center.x - swordWidth / 2.0f, center.y - swordHeight / 2.0f, center.z - swordDepth / 2.0f};
	aabb.max = {center.x + swordWidth / 2.0f, center.y + swordHeight / 2.0f, center.z + swordDepth / 2.0f};

	return aabb;
}

void Player::TakeDamage(int damage) {
	if (damageCoolTime_ > 0) {
		return;
	}
	hp_ -= damage;
	hp_ = std::max(hp_, 0);
	damageCoolTime_ = kDamageCoolTime;
}

Vector3 Player::GetHeadWorldPosition() const {
	AABB aabb = GetAABB();
	return {(aabb.min.x + aabb.max.x) * 0.5f, aabb.max.y, (aabb.min.z + aabb.max.z) * 0.5f};
}