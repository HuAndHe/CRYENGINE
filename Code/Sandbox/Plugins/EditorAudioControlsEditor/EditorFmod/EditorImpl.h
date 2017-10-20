// Copyright 2001-2016 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <IEditorImpl.h>

#include "ImplControls.h"

#include <CrySystem/File/CryFile.h>
#include <CryString/CryPath.h>

namespace ACE
{
namespace Fmod
{
class CImplSettings final : public IImplSettings
{
public:

	CImplSettings()
		: m_projectPath(PathUtil::GetGameFolder() + CRY_NATIVE_PATH_SEPSTR AUDIO_SYSTEM_DATA_ROOT CRY_NATIVE_PATH_SEPSTR "fmod_project")
		, m_soundBanksPath(PathUtil::GetGameFolder() + CRY_NATIVE_PATH_SEPSTR AUDIO_SYSTEM_DATA_ROOT CRY_NATIVE_PATH_SEPSTR "fmod")
	{}

	// IImplSettings
	virtual char const* GetSoundBanksPath() const override { return m_soundBanksPath.c_str(); }
	virtual char const* GetProjectPath() const override    { return m_projectPath.c_str(); }
	virtual void        SetProjectPath(char const* szPath) override;
	// ~IImplSettings

	void Serialize(Serialization::IArchive& ar);

private:

	string       m_projectPath;
	string const m_soundBanksPath;
};

class CEditorImpl final : public IEditorImpl
{
	friend class CProjectLoader;

public:

	CEditorImpl();
	virtual ~CEditorImpl() override;

	// IEditorImpl
	virtual void            Reload(bool const preserveConnectionStatus = true) override;
	virtual CImplItem*      GetRoot() override { return &m_rootControl; }
	virtual CImplItem*      GetControl(CID const id) const override;
	virtual char const*     GetTypeIcon(CImplItem const* const pImplItem) const override;
	virtual string          GetName() const override;
	virtual IImplSettings*  GetSettings() override { return &m_implSettings; }
	virtual bool            IsTypeCompatible(ESystemItemType const systemType, CImplItem const* const pImplItem) const override;
	virtual ESystemItemType ImplTypeToSystemType(CImplItem const* const pImplItem) const override;
	virtual ConnectionPtr   CreateConnectionToControl(ESystemItemType const controlType, CImplItem* const pImplItem) override;
	virtual ConnectionPtr   CreateConnectionFromXMLNode(XmlNodeRef pNode, ESystemItemType const controlType) override;
	virtual XmlNodeRef      CreateXMLNodeFromConnection(ConnectionPtr const pConnection, ESystemItemType const controlType) override;
	virtual void            EnableConnection(ConnectionPtr const pConnection) override;
	virtual void            DisableConnection(ConnectionPtr const pConnection) override;
	// ~IEditorImpl

private:

	void Clear();
	void CreateControlCache(CImplItem const* const pParent);

	CImplItem* CreateItem(EImpltemType const type, CImplItem* const pParent, string const& name);
	CID        GetId(EImpltemType const type, string const& name, CImplItem* const pParent) const;
	string     GetPathName(CImplItem const* const pImplItem) const;
	string     GetTypeName(EImpltemType const type) const;

	CImplItem* GetItemFromPath(string const& fullpath);
	CImplItem* CreatePlaceholderFolderPath(string const& path);

	typedef std::map<CID, CImplItem*> ControlsCache;
	typedef std::map<CID, int> ConnectionsMap;

	CImplItem      m_rootControl;
	ControlsCache  m_controlsCache; // cache of the controls stored by id for faster access
	ConnectionsMap m_connectionsByID;
	CImplSettings  m_implSettings;
};
} // namespace Fmod
} // namespace ACE