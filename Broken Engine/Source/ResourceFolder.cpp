#include "ResourceFolder.h"
#include "Application.h"
#include "ModuleGui.h"
#include "ModuleFileSystem.h"
#include "ModuleResourceManager.h"

#include "mmgr/mmgr.h"


ResourceFolder::ResourceFolder(uint UID, std::string source_file) : Resource(Resource::ResourceType::FOLDER, UID, source_file)
{
	extension = ".folder";
	resource_file = source_file;
	previewTexID = App->gui->folderTexID;

	std::string tmp = source_file;
	tmp.pop_back();
	App->fs->SplitFilePath(tmp.c_str(), nullptr, &tmp);
	tmp.append("/");
	name = tmp;
}

ResourceFolder::~ResourceFolder()
{
	resources.clear();
	childs.clear();
}

bool ResourceFolder::LoadInMemory()
{
	return true;
}

void ResourceFolder::FreeMemory()
{
}

void ResourceFolder::AddResource(Resource* resource)
{
	if(!HasResource(resource))
		resources.push_back(resource);
	else
		CONSOLE_LOG("![Warning]: Trying to add an already contained resource to folder: %s", this->GetName());
}

void ResourceFolder::RemoveResource(Resource* resource)
{
	for (std::vector<Resource*>::const_iterator it = resources.begin(); it != resources.end(); ++it)
	{
		if ((*it)->GetUID() == resource->GetUID())
		{
			resources.erase(it);
			break;
		}
	}
}

void ResourceFolder::SetParent(ResourceFolder* parent)
{
	if (parent)
	{
		this->parent = parent;
		parent->childs.push_back(this);
	}
}

void ResourceFolder::RemoveChild(ResourceFolder* child)
{
	for (std::vector<ResourceFolder*>::const_iterator it = childs.begin(); it != childs.end(); ++it)
	{
		if ((*it)->GetUID() == child->GetUID())
		{
			childs.erase(it);
			break;
		}
	}
}

std::vector<Resource*>& ResourceFolder::GetResources()
{
	return resources;
}

std::vector<ResourceFolder*>& ResourceFolder::GetChilds()
{
	return childs;
}

ResourceFolder* ResourceFolder::GetParent() const
{
	return parent;
}

bool ResourceFolder::HasResource(Resource* resource)
{
	bool found = false;

	for (std::vector<Resource*>::const_iterator it = resources.begin(); it != resources.end(); ++it)
	{
		if ((*it)->GetUID() == resource->GetUID())
		{
			found = true;
			break;
		}
	}

	return found;
}

void ResourceFolder::OnOverwrite()
{
	NotifyUsers(ResourceNotificationType::Overwrite);

}

void ResourceFolder::OnDelete()
{
	NotifyUsers(ResourceNotificationType::Deletion);

	for (uint i = 0; i < resources.size(); ++i)
	{
		resources[i]->OnDelete();
		delete resources[i];
	}

	App->fs->Remove(resource_file.c_str());

	App->resources->RemoveResourceFromFolder(this);
	App->resources->ONResourceDestroyed(this);

	if (parent)
		parent->RemoveChild(this);
}