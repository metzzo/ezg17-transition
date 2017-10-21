#pragma once
class IResource
{
public:
	virtual ~IResource() = default;
	virtual int get_resource_id() const = 0;
};

