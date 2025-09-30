#include "stdafx.h"
#include "DrawObject.h"
#include "DrawObjectManager.h"
#include "ServiceLocator/SingletonLocator.h"
#include "Common.h"

namespace
{
	void CheckOnTarget([[maybe_unused]] stn::DrawObject* target, [[maybe_unused]] stn::DrawObject* obj)
	{
		#ifdef _DEBUG
		if (obj == target)
		{
			// 自分に自分を設定している。
			// もしくは、子どもに自身を設定しようとしている。
			STN_DEBUG_BREAK(false);
            return;
		}
		for (auto child = obj->getFirstChild(); child; child = child->getNextSibling())
		{
			CheckOnTarget(target, child);
		}
		#endif // _DEBUG
	}
}

namespace stn
{
	DrawObject::DrawObject(Layer layer)
		: m_pParent{ nullptr }
		, m_pPrevSibling{ nullptr }
		, m_pNextSibling{ nullptr }
		, m_pFirstChild{ nullptr }
		, m_layer{ layer }
		, m_position{ Vec2::Zero() }
		, m_scale{ Vec2::One() }
		, m_rotateZ{ 0.0 }
		, m_isViewing{ true }
	{
		if (auto* pManager = STN_GET_LOCATOR(stn::DrawObjectManager).get())
		{
			pManager->addObject(layer, this);
		}
	}

	DrawObject::~DrawObject()
	{
		detach();
	}

	void DrawObject::draw() const
	{
	}

	void DrawObject::detach()
	{
		detachParentAndSibling();
		for (auto child = m_pFirstChild; child; child = child->m_pNextSibling)
		{
			child->m_pParent = nullptr;
		}
		m_pFirstChild = nullptr;
	}

	void DrawObject::detachParentAndSibling()
	{
		if (m_pPrevSibling)
		{
			m_pPrevSibling->m_pNextSibling = m_pNextSibling;
		}
		if (m_pNextSibling)
		{
			m_pNextSibling->m_pPrevSibling = m_pPrevSibling;
		}
		if (m_pParent)
		{
			if (m_pParent->m_pFirstChild == this)
			{
				if (m_pNextSibling)
				{
					m_pParent->m_pFirstChild = m_pNextSibling;
				}
				else
				{
					m_pParent->m_pFirstChild = nullptr;
				}
			}
		}
		m_pPrevSibling = nullptr;
		m_pNextSibling = nullptr;
		m_pParent = nullptr;
	}

	void DrawObject::setPrev(DrawObject* obj)
	{
		CheckOnTarget(this, obj);
		obj->detachParentAndSibling();
		if (m_pPrevSibling)
		{
			m_pPrevSibling->m_pNextSibling = obj;
			obj->m_pPrevSibling = m_pPrevSibling;
		}
		else if (m_pParent)
		{
			m_pParent->m_pFirstChild = obj;
		}
		this->m_pPrevSibling = obj;
		obj->m_pNextSibling = this;
		obj->m_pParent = m_pParent;
	}

	void DrawObject::setNext(DrawObject* obj)
	{
		CheckOnTarget(this, obj);
		obj->detachParentAndSibling();
		if (m_pNextSibling)
		{
			m_pNextSibling->m_pPrevSibling = obj;
			obj->m_pNextSibling = m_pNextSibling;
		}
		this->m_pNextSibling = obj;
		obj->m_pPrevSibling = this;
		obj->m_pParent = m_pParent;
	}

	void DrawObject::addChild(DrawObject* obj)
	{
		CheckOnTarget(this, obj);
		obj->detachParentAndSibling();
		if (m_pFirstChild)
		{
			m_pFirstChild->getLastSibling()->setNext(obj);
		}
		else
		{
			m_pFirstChild = obj;
		}
		obj->m_pParent = this;
	}

	DrawObject* DrawObject::getFirstSibling()
	{
		for (auto prev = m_pPrevSibling; prev;)
		{
			if (prev->m_pPrevSibling == nullptr)
			{
				return prev;
			}
			prev = prev->m_pPrevSibling;
		}
		return this;
	}

	DrawObject* DrawObject::getLastSibling()
	{
		for (auto next = m_pNextSibling; next;)
		{
			if (next->m_pNextSibling == nullptr)
			{
				return next;
			}
			next = next->m_pNextSibling;
		}
		return this;
	}

	DrawObject* DrawObject::getParent() const
	{
		return m_pParent;
	}

	DrawObject* DrawObject::getPrevSibling() const
	{
		return m_pPrevSibling;
	}

	DrawObject* DrawObject::getNextSibling() const
	{
		return m_pNextSibling;
	}

	DrawObject* DrawObject::getFirstChild() const
	{
		return m_pFirstChild;
	}

	void DrawObject::setPosition(double x, double y)
	{
		setPosition(Vec2{ x, y });
	}

	void DrawObject::setPosition(const Vec2& position)
	{
		m_position = position;
	}

	const Vec2& DrawObject::getPosition() const
	{
		return m_position;
	}

	const Vec2 DrawObject::getWorldPosition() const
	{
		if (m_pParent)
		{
			return (m_position + m_pParent->getWorldPosition());
		}
		return m_position;
	}

	void DrawObject::setScale(double x, double y)
	{
		setScale(Vec2{ x, y });
	}

	void DrawObject::setScale(const Vec2& scale)
	{
		m_scale = scale;
	}

	const Vec2& DrawObject::getScale() const
	{
		return m_scale;
	}

	const Vec2 DrawObject::getWorldScale() const
	{
		if (m_pParent)
		{
			return (m_scale * m_pParent->getWorldScale());
		}
		return m_scale;
	}

	void DrawObject::setRotateZ(double rotateZ)
	{
		m_rotateZ = rotateZ;
	}

	double DrawObject::getRotateZ() const
	{
		return m_rotateZ;
	}

	double DrawObject::getWorldRotateZ() const
	{
		if (m_pParent)
		{
			return (m_rotateZ + m_pParent->getWorldRotateZ());
		}
		return m_rotateZ;
	}

	void DrawObject::setViewing(bool isViewing)
	{
		m_isViewing = isViewing;
	}

	bool DrawObject::isViewing() const
	{
		return m_isViewing;
	}

	void DrawObject::setName(FMT_MAYBE_UNUSED StringView)
	{
	}

	StringView DrawObject::getName() const
	{
		return U"";
	}

	void DrawObject::moveX(double x)
	{
		move(Vec2{ x, 0.0 });
	}

	void DrawObject::moveY(double y)
	{
		move(Vec2{ 0.0, y });
	}

	void DrawObject::move(double x, double y)
	{
		move(Vec2{ x, y });
	}

	void DrawObject::move(const Vec2& move)
	{
		m_position += move;
	}
}
