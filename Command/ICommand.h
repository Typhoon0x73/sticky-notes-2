#pragma once
#include <Siv3D.hpp>

namespace stn
{
	/**
	* @brief コマンド抽象クラス
	*/
	class ICommand
	{
	public:

		/**
		* @brief デストラクタ
		*/
		virtual ~ICommand() = default;

		/**
		* @brief 実行
		* @return true : 成功, false : 失敗
		*/
		virtual bool execute() = 0;

		/**
		* @brief 元に戻す
		* @return true : 成功, false : 失敗
		*/
		virtual bool undo() = 0;

		/**
		* @brief やり直し
		* @return true : 成功, false : 失敗
		*/
		virtual bool redo() = 0;
	};
}

