#pragma once
#ifndef _SMART_POINTER_H_
#define _SMART_POINTER_H_
#include <type_traits>

namespace RSY_TOOL
{

	namespace Smart_Pointer
	{

		/*
		 * Template class for Referencing Counting Object
		**/
		template<typename _Ty>
		class Ref_Count_Obj
		{

		public:

			Ref_Count_Obj(const Ref_Count_Obj& other)
				:refCount(other.refCount), pointee(other.pointee)
			{
				if (pointee)addReference();
			}

			template<typename _Ty2>
			Ref_Count_Obj(const Ref_Count_Obj<_Ty2>& other)
				: refCount(other.refCount), pointee(other.pointee)
			{
				if (pointee)addReference();
			}

			Ref_Count_Obj(Ref_Count_Obj&& other)
				:refCount(other.refCount), pointee(other.pointee)
			{
				other.refCount = nullptr;
				other.pointee = nullptr;
			}

			Ref_Count_Obj& operator=(const Ref_Count_Obj& other)
			{
				if (pointee)removeReference();
				this->refCount = other.refCount;
				this->pointee = other.pointee;
				if (pointee)addReference();
				return *this;
			}

			Ref_Count_Obj& operator=(Ref_Count_Obj&& other)
			{
				if (pointee)removeReference();
				this->refCount = other.refCount;
				this->pointee = other.pointee;
				other.refCount = nullptr;
				other.pointee = nullptr;
				return *this;
			}

			explicit operator bool() const noexcept { return pointee != nullptr; }

			void addReference() { ++*refCount; }
			void removeReference() {
				if (--*refCount == 0) {
					delete pointee;
					delete refCount;
				}
			}

			void destroy() { if (pointee)removeReference(); }

			_Ty* operator->() const noexcept { return pointee; }
			_Ty* get() const noexcept { return pointee; }
			_Ty& operator*() const noexcept { return *pointee; }


		private:

			template<typename _Ty>
			friend class SmartPtr;

			/* WTF, this violates with template copy ctor
			template<class... _Types>
			Ref_Count_Obj(_Types&&... _Args)
				:refCount(new int(1)),
				pointee(new _Ty(std::forward<_Types>(_Args)...))
			{}
			*/

			Ref_Count_Obj()
				:refCount(nullptr), pointee(nullptr)
			{}

			Ref_Count_Obj(_Ty* ptr)
				: refCount(new int(1)), pointee(ptr)
			{}

			int* refCount;
			_Ty* pointee;


		};//end template class Ref_Count_Obj


		/*
		 * Template class for Smart Pointer
		**/
		template<typename _Ty>
		class SmartPtr : public Ref_Count_Obj<_Ty>
		{

		public:

			/*
			 * trivial constructor
			**/
			SmartPtr()
				:Ref_Count_Obj() {}

			SmartPtr(_Ty* ptr)
				:Ref_Count_Obj(ptr) {}

			template<class... _Types>
			SmartPtr(_Types&&..._Args)
				: Ref_Count_Obj(std::forward<_Types>(_Args)...)
			{}

			/*
			 * Copy Constructor
			**/
			SmartPtr(const SmartPtr& other)
				: Ref_Count_Obj(other)
			{}


			/*
			 * Template Copy Constructor
			**/
			template<typename _Ty2>
			SmartPtr(const SmartPtr<_Ty2>& other)
				:Ref_Count_Obj(other)
			{}


			/*
			 * Move Constructor
			**/
			SmartPtr(SmartPtr&& other)
				:Ref_Count_Obj(std::forward<Ref_Count_Obj>(other))
			{}


			/*
			 * Copy Assignment
			**/
			SmartPtr& operator=(const SmartPtr& other)
			{
				Ref_Count_Obj::operator=(other);
				return *this;
			}


			/*
			 * Move Assignment
			**/
			SmartPtr& operator=(SmartPtr&& other)
			{
				Ref_Count_Obj::operator=(std::forward<Ref_Count_Obj>(other));
				return *this;
			}


			/*
			 * Dereferencing Operator
			**/
			_Ty& operator*() const
			{
				return Ref_Count_Obj::operator*();
			}
			_Ty* operator->() const noexcept
			{
				//p->foo();
				//=>
				//(p.operaotr->())->foo();
				return Ref_Count_Obj::operator->();
			}


			explicit operator bool() const noexcept
			{
				return pointee != nullptr;
			}


			/*
			 * get dump pointer
			**/
			_Ty* get() const noexcept
			{
				return this->operator*();
			}


			/*
			 * Destructor
			**/
			~SmartPtr()
			{
				this->destroy();
			}

		};//end template class SmartPtr


		/*
		 * Template Function make_smart
		**/
		template<class _Ty,
			class... _Types> inline
			SmartPtr<_Ty> make_smart(_Types&&..._Args)
		{
			return SmartPtr<_Ty>(std::forward<_Ty>(_Args)...);
		}



	}//end namespace Smart_Pointer

}//end namespace RSY_TOOL

#endif // !_SMART_POINTER_H_
