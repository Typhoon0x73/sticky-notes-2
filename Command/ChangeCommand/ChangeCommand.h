#pragma once
#include "../ICommand.h"

namespace stn
{
	template < typename T >
	class ChangeCommand : public ICommand
	{
	public:
		/**
		* @brief コンストラクタ
		* @param obj 変更対象
		* @param val 変更後値
		*/
		explicit ChangeCommand(T* obj, const T& val)
			: m_pObject{ obj }
			, m_value{ val }
			, m_prevValue{}
		{
			if (m_pObject)
			{
				m_prevValue = *m_pObject;
			}
		}

		/**
		* @brief デストラクタ
		*/
		virtual ~ChangeCommand() = default;

		/**
		* @brief 実行
		* @return true : 成功, false : 失敗
		*/
		virtual bool execute() override
		{
			return false;
		}

		/**
		* @brief 元に戻す
		* @return true : 成功, false : 失敗
		*/
		virtual bool undo() override
		{
			return false;
		}

		/**
		* @brief やり直し
		* @return true : 成功, false : 失敗
		*/
		virtual bool redo() override
		{
			return false;
		}

	private:

		T  m_prevValue;  //!< 変更前
		T  m_value;      //!< 変更後
		T* m_pObject;    //!< 変更対象
	};


#define ChangeCommandInlineDefault(TYPE)                 \
	template <>											 \
	inline virtual bool ChangeCommand<TYPE>::execute()	 \
	{													 \
		if (m_pObject == nullptr)						 \
		{												 \
			return false;								 \
		}												 \
		*m_pObject = m_value;							 \
		return true;									 \
	}													 \
														 \
	template <>											 \
	inline virtual bool ChangeCommand<TYPE>::undo()		 \
	{													 \
		if (m_pObject == nullptr)						 \
		{												 \
			return false;								 \
		}												 \
		*m_pObject = m_prevValue;						 \
		return true;									 \
	}													 \
														 \
	template <>											 \
	inline virtual bool ChangeCommand<TYPE>::redo()      \
	{													 \
		return execute();								 \
	}

	using ChangeBooleanCommand = ChangeCommand<bool>;
	using ChangeS8Command = ChangeCommand<int8>;
	using ChangeS16Command = ChangeCommand<int16>;
	using ChangeS32Command = ChangeCommand<int32>;
	using ChangeS64Command = ChangeCommand<int64>;
	using ChangeU8Command = ChangeCommand<uint8>;
	using ChangeU16Command = ChangeCommand<uint16>;
	using ChangeU32Command = ChangeCommand<uint32>;
	using ChangeU64Command = ChangeCommand<uint64>;
	using ChangeFloatCommand = ChangeCommand<float>;
	using ChangeDoubleCommand = ChangeCommand<double>;
	using ChangeColorFCommand = ChangeCommand<ColorF>;
	using ChangeRectFCommand = ChangeCommand<RectF>;
	using ChangeStringCommand = ChangeCommand<std::string>;
	using ChangeSivStringCommand = ChangeCommand<String>;

	ChangeCommandInlineDefault(bool)
	ChangeCommandInlineDefault(int8)
	ChangeCommandInlineDefault(int16)
	ChangeCommandInlineDefault(int32)
	ChangeCommandInlineDefault(int64)
	ChangeCommandInlineDefault(uint8)
	ChangeCommandInlineDefault(uint16)
	ChangeCommandInlineDefault(uint32)
	ChangeCommandInlineDefault(uint64)
	ChangeCommandInlineDefault(float)
	ChangeCommandInlineDefault(double)
	ChangeCommandInlineDefault(ColorF)
	ChangeCommandInlineDefault(RectF)
	ChangeCommandInlineDefault(std::string)
	ChangeCommandInlineDefault(String)
}


