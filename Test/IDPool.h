#pragma once
#include <string>
#include "CircularQueue.h"

class IDPool
{
public:
	IDPool(const char* type_desc, uint32 valuemax) : pool_des_(type_desc), value_max_(valuemax), cur_max_id_(0){}
	~IDPool(void){}

	bool id_alloc(uint32& allocid);

	void free_id(uint32 allocid);

	std::string						pool_des_;
	uint32							value_max_;
	uint32							cur_max_id_;
	CircularQueue< uint32, 1024>	free_allocids_;
};

