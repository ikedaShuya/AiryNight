#pragma once

#include <vector>

// ステージの配置データ
class StagePlacementData {
public:
	// 敵の配置座標
	struct EnemyPosition {
		int x;
		int y;
	};

	// 敵の配置を取得
	const std::vector<EnemyPosition>& GetEnemyPositions() const { return enemyPositions_; }

	// 敵の配置を追加
	void AddEnemyPosition(int x, int y) { enemyPositions_.push_back({x, y}); }

	// 指定した座標の敵の配置を削除
	void RemoveEnemyPosition(int x, int y) {
		for (auto it = enemyPositions_.begin(); it != enemyPositions_.end(); ++it) {
			if (it->x == x && it->y == y) {
				enemyPositions_.erase(it);
				return;
			}
		}
	}

	// 指定した座標に敵が配置されているか確認
	bool HasEnemyPosition(int x, int y) const {
		for (const EnemyPosition& position : enemyPositions_) {
			if (position.x == x && position.y == y) {
				return true;
			}
		}
		return false;
	}

	// 敵の配置をクリア
	void ClearEnemyPositions() { enemyPositions_.clear(); }

	// プレイヤーの配置を取得
	int GetPlayerX() const { return playerX_; }
	int GetPlayerY() const { return playerY_; }

	// プレイヤーの配置を設定
	void SetPlayerPosition(int x, int y) {
		playerX_ = x;
		playerY_ = y;
	}

	// 旗の配置を取得
	int GetFlagX() const { return flagX_; }
	int GetFlagY() const { return flagY_; }

	// 旗の配置を設定
	void SetFlagPosition(int x, int y) {
		flagX_ = x;
		flagY_ = y;
	}

private:
	// 敵の配置
	std::vector<EnemyPosition> enemyPositions_;

	// プレイヤーの配置
	int playerX_ = 1;
	int playerY_ = 12;

	// 旗の配置
	int flagX_ = 97;
	int flagY_ = 12;
};