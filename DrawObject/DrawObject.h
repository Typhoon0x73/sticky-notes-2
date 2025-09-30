#pragma once

// INCLUDE
#include "LayerDefine.h"

namespace stn
{

	class DrawObject
	{
	public:

		/// @brief コンストラクタ
		/// @param layer 登録するレイヤー
		DrawObject(Layer layer);

		/// @brief デストラクタ
		virtual ~DrawObject();

		/// @brief 描画 
		virtual void draw() const;
		
		/// @brief 関係の破棄
		/// 子どもの参照も切るので子どもがついている場合、迷子になる
		/// 必要な場合は先に子どもを確保すること
		void detach();

		/// @brief 関係の破棄
		/// 子どもはそのまま保持、親と兄弟との関係を切る
		void detachParentAndSibling();

		/// @brief 自身の前に設定する
		/// @param obj 設定するオブジェクト
		void setPrev(DrawObject* obj);

		/// @brief 自身の次に設定する
		/// @param obj 設定するオブジェクト
		void setNext(DrawObject* obj);

		/// @brief 自分の子どもに設定する
		/// @param obj 設定するオブジェクト
		void addChild(DrawObject* obj);

		/// @brief 長男の取得
		/// @return 長男オブジェクト
		DrawObject* getFirstSibling();

		/// @brief 末っ子の取得
		/// @return 末っ子オブジェクト
		DrawObject* getLastSibling();

		/// @brief 親の取得
		/// @return 親
		DrawObject* getParent() const;

		/// @brief 前のオブジェクトを取得
		/// @return 前のオブジェクト
		DrawObject* getPrevSibling() const;

		/// @brief 次のオブジェクトを取得
		/// @return 次のオブジェクト
		DrawObject* getNextSibling() const;

		/// @brief 子どもの長男を取得
		/// @return 子どもの長男
		DrawObject* getFirstChild() const;

		/// @brief 表示座標の設定
		/// @param x 
		/// @param y 
		virtual void setPosition(double x, double y);

		/// @brief 表示座標の設定
		/// @param position 
		virtual void setPosition(const Vec2& position);

		virtual const Vec2& getPosition() const;
		virtual const Vec2 getWorldPosition() const;

		virtual void setScale(double x, double y);
		virtual void setScale(const Vec2& scale);
		virtual const Vec2& getScale() const;
		virtual const Vec2 getWorldScale() const;

		virtual void setRotateZ(double rotateZ);
		virtual double getRotateZ() const;
		virtual double getWorldRotateZ() const;

		virtual void setViewing(bool isViewing);
		virtual bool isViewing() const;

		virtual void setName(StringView name);
		virtual StringView getName() const;

		virtual void moveX(double x);
		virtual void moveY(double y);
		virtual void move(double x, double y);
		virtual void move(const Vec2& move);


	protected:

		DrawObject* m_pParent; //!< 親
		DrawObject* m_pPrevSibling; //!< 前のオブジェクト
		DrawObject* m_pNextSibling; //!< 次のオブジェクト
		DrawObject* m_pFirstChild; //!< 子どもの長男

		Layer m_layer; //!< 自身のレイヤー
		Vec2 m_position; //!< 座標
		Vec2 m_scale; //!< 拡縮
		double m_rotateZ; //!< 回転
		bool m_isViewing; //!< 表示フラグ
	};
}

