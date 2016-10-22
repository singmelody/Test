#pragma once
class GIDMap
{
public:
	typedef HashTable< int32, VType, 1024>	MapType;
	typedef int32 (VType::*GetKeyFun)();

	enum{
		InvalidID = 0,
	};

	class Iterator
	{
		friend class GIDMap<VType,SlotType>

	public:
		Iterator(const GIDMap* id_map = NULL, SlotType* slot = NULL):id_map_(id_map),slot_(slot)
		{

		}

		Iterator& operator++()
		{
			AvalonAssert(id_map_);

			Iterator itr = Iterator(id_map_->next(*this));
			slot_ = itr.slot_;
			id_map_ = itr.id_map_;

			return *const_cast<Iterator*>(this);
		}

		VType* operator*() const
		{
			AvalonAssert(slot_);
			return slot_->GetContent();
		}

		bool operator==(const Iterator& itr)
		{
			return id_map_ == itr.id_map_ && slot_ == itr.slot_;
		}

		bool operator!=(const Iterator& itr)
		{
			return !(id_map_==itr.id_map_ && slot_ == itr.slot_);
		}

	protected:
		SlotType*		slot_;
		const GIDMap*	id_map_;
	};

	GIDMap( GetKeyFun keyfun, uint32 gen_key_max = 0xffffffff): free_keys("GIDMap", gen_key_max), _get_key(keyfun), 
	~GIDMap(void) {}

	Iterator begin()
	{
		for (int32 i = 0; i < id_map_.GetHashTableSize(); ++i)
		{
			typename GHashTable< int32, VType, 1024>::Slot* pSlot = id_map_.GetData(i);
			while (pSlot)
			{
				if(pSlot->GetState() != HashTable<int32, VType, 1024>::IDTS_EMPTY)
					return Iterator( this, pSlot);
			}
		}


		return Iterator(this);
	}

	Iterator end()
	{
		return Iterator(this);
	}

	Iterator next()
	{
		AvalonAssert( itr.id_map == this && itr.slot_ );
		return Iterator( this, id_map_.next(itr.slot_));
	}

	bool insert( VType* elem, Iterator* ret_itr = NULL)
	{
		if( elem == NULL)
			return false;

		int32 key_id = (elem->*_get_key)();

		typename HashTable<int32, VType, 1024>::Slot* pSlot = id_map_.PushContent( key_id, elem);
		if(!pSlot)
			return false;

		if(ret_itr)
		{
			ret_itr->id_map_ = this;
			ret_itr->slot_ = pSlot;
		}

		return true;
	}

	Iterator find(int32 key)
	{
		return Iterator( this, id_map_.FindUseSlotByID(key));
	}

	Iterator erase(const Iterator& itr)
	{
		AvalonAssert( itr.id_map_ == this && itr.slot_ );
		VType* elem = itr.slot_->GetContent();

		free_keys_.free_ID((elem->*_get_key)());
		return Iterator( this, id_map_.RemoveSlot(key));
	}

	Iterator erase(int32 key)
	{
		free_keys_.free_id(key);
		return Iterator( this, id_map_.RemoveSlot(key));
	}

	int32 gen_key()
	{
		uint32 key = InvalidID;
		if(!free_keys_.id_alloc(key))
			return cur_gen_key_< genkey_max ? ++cur_gen_key:InvalidID;

		return key;
	}

	protected:
		uint32		cur_gen_key_;
		uint32		gen_key_max_;
		IDPool		free_keys_;
		HashTable< int32, VType, 1024>	id_maps;
		GetKeyFun	_get_key;
};

