/* Copyright (C) 2019 Wildfire Games.
 * This file is part of 0 A.D.
 *
 * 0 A.D. is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * 0 A.D. is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 0 A.D.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "precompiled.h"

#include "CGUISetting.h"

#include "gui/CGUI.h"
#include "ps/CLogger.h"

template<typename T>

CGUISetting<T>::CGUISetting(IGUIObject& pObject, const CStr& Name, T& Value)
	: m_pSetting(Value), m_Name(Name), m_pObject(pObject)
{
}

template<typename T>
bool CGUISetting<T>::FromString(const CStrW& Value, const bool SendMessage)
{
	T settingValue;

	if (!CGUI::ParseString<T>(&m_pObject.GetGUI(), Value, settingValue))
		return false;

	m_pObject.SetSetting<T>(m_Name, settingValue, SendMessage);
	return true;
};

template<>
bool CGUISetting<CGUIColor>::FromJSVal(const ScriptRequest& rq, JS::HandleValue Value, const bool SendMessage)
{
	CGUIColor settingValue;
	if (Value.isString())
	{
		CStr name;
		if (!ScriptInterface::FromJSVal(rq, Value, name))
			return false;

		if (!settingValue.ParseString(m_pObject.GetGUI(), name))
		{
			LOGERROR("Invalid color '%s'", name.c_str());
			return false;
		}
	}
	else if (!ScriptInterface::FromJSVal<CColor>(rq, Value, settingValue))
		return false;

	m_pObject.SetSetting<CGUIColor>(m_Name, settingValue, SendMessage);
	return true;
};

template<typename T>
bool CGUISetting<T>::FromJSVal(const ScriptRequest& rq, JS::HandleValue Value, const bool SendMessage)
{
	T settingValue;
	if (!ScriptInterface::FromJSVal<T>(rq, Value, settingValue))
		return false;

	m_pObject.SetSetting<T>(m_Name, settingValue, SendMessage);
	return true;
};

template<typename T>
void CGUISetting<T>::ToJSVal(const ScriptRequest& rq, JS::MutableHandleValue Value)
{
	ScriptInterface::ToJSVal<T>(rq, Value, m_pSetting);
};

#define TYPE(T) \
	template class CGUISetting<T>; \

#include "gui/GUISettingTypes.h"
#undef TYPE
