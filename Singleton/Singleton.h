#pragma once

// INCLUDE
#include <cassert>
#include <memory>

namespace stn
{
    /**
     * @brief シングルトンアクセス汎用クラス
     * (継承して使う)
     * @tparam T 継承先クラス
     */
    template < typename T >
    class Singleton
    {
    public:
        /**
         * @brief Get the Instance object
         * 
         * @return constexpr T& シングルトンインスタンス
         */
        static constexpr T& getInstance()
        {
            if (m_pInstance == nullptr)
            {
                create();
            }
            return *m_pInstance.get();
        }

        /**
         * @brief シングルトンインスタンスの破棄
         * 
         */
        static void release()
        {
            if (m_pInstance)
            {
				m_pInstance.reset();
            }
        }

    protected:

        /**
         * @brief Construct a new Singleton object
         * 
         */
        Singleton() {}

        /**
         * @brief Destroy the Singleton object
         * 
         */
        virtual ~Singleton() {}

        /**
         * @brief シングルトンインスタンスの生成
         * 
         */
        static void create()
        {
            assert(!m_pInstance);
            if (m_pInstance)
            {
                return;
            }
            m_pInstance.reset(new T());
        }

    private:

        /*! コピー、ムーブコンストラクタの削除 */
        Singleton(const Singleton&) = delete;
        Singleton(Singleton&&) = delete;

        static std::unique_ptr<T> m_pInstance; //!< シングルトンインスタンス
    };

    /*! staticメンバの初期化 */
    template < typename T >
    std::unique_ptr<T> Singleton<T>::m_pInstance = nullptr;
}
