#pragma once
#include "ICommand.h"

namespace stn
{
	/**
	* @brief コマンド管理クラス
	*/
	class CommandManager
	{
	public:

		/**
		* @brief コンストラクタ
		*/
		explicit CommandManager();

		/**
		* @brief デストラクタ
		*/
		virtual ~CommandManager();

		void refresh();

		/**
		* @brief コマンドの登録
		* @param command コマンドポインタ
		* @return true : 成功, false : 失敗
		*/
		bool registry(std::unique_ptr<ICommand>&& command);

		/**
		* @brief コマンド履歴の削除
		*/
		void clear();

		/**
		* @brief 元に戻す
		* @return true : 成功, false : 失敗
		*/
		bool undo();

		/**
		* @brief やり直し
		* @return true : 成功, false : 失敗
		*/
		bool redo();

		/**
		* @brief 実行コマンドリストの取得
		* @return 実行コマンドリスト
		*/
		const Array<std::unique_ptr<ICommand>>& executeList() const;

		/**
		* @brief 元に戻すコマンドリストの取得
		* @return 元に戻すコマンドリスト
		*/
		const Array<std::unique_ptr<ICommand>>& undoList() const;

		/**
		* @brief コマンド記憶最大数の再設定(def:50)
		* @param capacity コマンド記憶最大数
		*/
		void setExecuteCommandCapacity(size_t capacity = 50);

		bool isExecuted() const;

	private:

		size_t								m_capacity{ 50 };		//!< コマンド記憶最大数
		Array<std::unique_ptr<ICommand>>	m_executes{};			//!< コマンド実行リスト
		Array<std::unique_ptr<ICommand>>	m_undoList{}	;		//!< コマンド元に戻すリスト
		bool								m_isExecuted{ false };	//!< このフレームで実行されたコマンドがあるかのフラグ確認
	};

}
