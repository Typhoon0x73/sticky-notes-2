#include "CommandManager.h"

namespace stn
{
	/**
	* @brief コンストラクタ
	*/
	CommandManager::CommandManager()
		: m_capacity{ 50 }
		, m_executes{}
		, m_undoList{}
		, m_isExecuted{ false }
	{
	}

	/**
	* @brief デストラクタ
	*/
	CommandManager::~CommandManager()
	{
		clear();
	}

	void CommandManager::refresh()
	{
		m_isExecuted = false;
	}

	/**
	* @brief コマンドの登録
	* @param command コマンドポインタ
	* @return true : 成功, false : 失敗
	*/
	bool CommandManager::registry(std::unique_ptr<ICommand>&& command)
	{
		if (!command->execute())
		{
			return false;
		}
		m_executes.push_back(std::move(command));
		m_undoList.clear();
		while (m_capacity < m_executes.size())
		{
			m_executes.pop_front();
		}
		m_isExecuted = true;
		return true;
	}

	/**
	* @brief コマンド履歴の削除
	*/
	void CommandManager::clear()
	{
		m_executes.clear();
		m_undoList.clear();
	}

	/**
	* @brief 元に戻す
	* @return true : 成功, false : 失敗
	*/
	bool CommandManager::undo()
	{
		if (m_executes.size() == 0)
		{
			return false;
		}
		auto command = std::move(m_executes.back());
		m_executes.pop_back();
		if (!command->undo())
		{
			return false;
		}
		m_undoList.emplace_back(std::move(command));
		m_isExecuted = true;
		return true;
	}

	/**
	* @brief やり直し
	* @return true : 成功, false : 失敗
	*/
	bool CommandManager::redo()
	{
		if (m_undoList.size() == 0)
		{
			return false;
		}
		auto command = std::move(m_undoList.back());
		m_undoList.pop_back();
		if (!command->redo())
		{
			return false;
		}
		m_executes.emplace_back(std::move(command));
		m_isExecuted = true;
		return true;
	}

	/**
	* @brief 実行コマンドリストの取得
	* @return 実行コマンドリスト
	*/
	const Array<std::unique_ptr<ICommand>>& CommandManager::executeList() const
	{
		return m_executes;
	}

	/**
	* @brief 元に戻すコマンドリストの取得
	* @return 元に戻すコマンドリスト
	*/
	const Array<std::unique_ptr<ICommand>>& CommandManager::undoList() const
	{
		return m_undoList;
	}

	/**
	* @brief コマンド記憶最大数の再設定(def:50)
	* @param capacity コマンド記憶最大数
	*/
	void CommandManager::setExecuteCommandCapacity(std::size_t capacity)
	{
		m_capacity = capacity;
		while (m_capacity < m_executes.size())
		{
			m_executes.pop_front();
		}
	}

	bool CommandManager::isExecuted() const
	{
		return m_isExecuted;
	}

}
