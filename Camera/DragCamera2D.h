#pragma once
#include <Siv3D.hpp>

namespace stn
{

	class DragCamera2D : public BasicCamera2D
	{
		using super = BasicCamera2D;

	public:

		/// @brief デフォルトコンストラクタ
		SIV3D_NODISCARD_CXX20
		DragCamera2D() = default;

		/// @brief 指定した設定で 2D カメラを作成します。
		/// @param center カメラが見ている中心座標の初期設定
		/// @param scale カメラのズーム倍率の初期設定
		SIV3D_NODISCARD_CXX20
		explicit constexpr DragCamera2D(const Vec2& center, double scale = 1.0) noexcept;

		virtual ~DragCamera2D();

		/// @brief 2D カメラの状態を更新します。
		/// @param deltaTime 前回のフレームからの経過時間（秒）
		/// @param sceneSize レンダーターゲットのサイズ（ピクセル）
		virtual void update(bool isMoveMouseL, double deltaTime = Scene::DeltaTime());

		/// @brief 指定した中心座標とズーム倍率を即座に適用します。
		/// @param center カメラが見る中心座標
		/// @param scale カメラのズーム倍率
		void jumpTo(Vec2 center, double scale) noexcept;

	protected:

		double m_targetScale = super::m_scale;

		double m_scaleChangeVelocity = 0.0;

		void updateWheel();

		void updateMouse(bool isMoveMouseL);
	};

}
