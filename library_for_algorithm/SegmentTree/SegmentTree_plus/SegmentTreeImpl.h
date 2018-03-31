#pragma once
#ifndef _SEGMENTTREEIMPL_H
#define _SEGMENTTREEIMPL_H
#include "SegmentTreeNode.h"
#include "SegmentTreeException.h"
#include <vector>
#include <algorithm>
using std::vector;

namespace RSY_TOOL
{

	namespace MySegmentTree
	{

		typedef struct {} Function;
		typedef struct :public Function {} Homeomorphism;
		typedef struct :public Function {} NoHomeomorphism;

#define __HOMEOMORPHISM_FUNC_ Homeomorphism{} 


		//SegmentTreeImpl Template
		template<class _Ty> class SegmentTreeImpl :public SegmentTreeType<_Ty>
		{


		public:

			//customized function constructor
			SegmentTreeImpl(const vector<_Ty>& Vec, const Func& func, const _Ty& Identity_Element)
				: _size(1),
				_Func(func),
				_Identity_Element(Identity_Element)
			{

				//空检查
				if (Vec.empty())
					throw SegmentTreeException<_Ty>("The Segment is empty!!");


				//初始化线段树，空间复杂度O(n)，时间复杂度O(n)
				find_size(Vec.size());
				ST.resize(2 * _size - 1);
				build(0, 0, _size - 1, Vec);

				aug.resize(_size - 1);
				std::for_each(aug.begin(), aug.end(),
					[&Identity_Element](_Ty& val) {val = Identity_Element; });

				value_cache.resize(_size - 1);
				std::for_each(value_cache.begin(), value_cache.end(),
					[&Identity_Element](_STD pair<_Ty, cache>& val)
				{val = _STD make_pair(Identity_Element, false); });

			}


			const _Ty query(const int start, const int end)
			{

				if (start > end)
					throw SegmentTreeException<_Ty>("The querying range is invalid!!");

				adjust(0, _Identity_Element);

				return doQuery(0, start, end);

			}


			//const member function
			const _Ty query(int start, int end) const
			{

				if (!_checked)
					throw SegmentTreeException<_Ty>("The Segment is empty!!");


				if (start > end)
					throw SegmentTreeException<_Ty>("The querying range is invalid!!");


				return doQuery(0, start, end);

			}


			void modify(const int start, const int end, const _Ty& value)
			{

				if (start < ST[0]->start() || end > ST[0]->end())
					throw SegmentTreeException<_Ty>("The Index is invalid!!");

				//adjust(0, _Identity_Element);

				doModify(0, start, end, value);

			}


			void modify_augment(const int start, const int end, const _Ty& aug_value)
			{

				if (start < ST[0]->start() || end > ST[0]->end() || start > end)
					throw SegmentTreeException<_Ty>("The modified range is invalid!!");

				//adjust(0, _Identity_Element);

				if (aug_value == _Identity_Element)return;

				doModify_augment(0, start, end, aug_value);

			}


			//NoHomeomorphism
			void modify(const int start, const int end,
				const modify_func& func, NoHomeomorphism = {})
			{

				if (start < ST[0]->start() || end > ST[0]->end())
					throw SegmentTreeException<_Ty>("The Index is invalid!!");

				//adjust(0, _Identity_Element);

				doModify(0, start, end, func);

			}


			//Homeomorphism
			void modify(const int start, const int end,
				const modify_func& func, Homeomorphism)
			{

				if (start < ST[0]->start() || end > ST[0]->end())
					throw SegmentTreeException<_Ty>("The Index is invalid!!");

				//adjust(0, _Identity_Element);

				doModify(0, start, end, func, __HOMEOMORPHISM_FUNC_);

			}



			SegmentTreeImpl(const SegmentTreeImpl&) = delete;

			SegmentTreeImpl& operator=(const SegmentTreeImpl&) = delete;

			SegmentTreeImpl(SegmentTreeImpl&&) = delete;

			SegmentTreeImpl& operator=(SegmentTreeImpl&&) = delete;

			~SegmentTreeImpl() = default;


		protected:


			//线段树最底层长度，总长度 (2 * _size - 1)
			int _size;


			//Segment_Tree with size of (2 * _size - 1)
			vector<SegmentTreeNode_ptr> ST;


			//accumulation for lazy evaluation with size of (_size - 1)
			//aug[index] represents the value that should be later
			//	applied to the any of the values below.
			vector<_Ty> aug;


			//value cache:
			//store the value that represents the any element in the range
			//it should be updated when necessary
			vector<_STD pair<_Ty, cache> > value_cache;


			//_Func是一个_Ty上的二元代数运算符，满足结合律、交换律，有幺元。
			//_Ty对_Func构成一个 可交换幺半群
			const Func _Func;


			//幺元
			const _Ty _Identity_Element;


		private:


			//find_size
			void find_size(const int size)
			{
				while (_size < size)
				{
					_size <<= 1;
				}
			}


			//SegmentTree Initialization
			void build(int index, int start, int end, const vector<_Ty>& Vec)
			{

				//leaf node
				if (start == end)
				{
					ST[index] = make_shared<SegmentTreeNode<_Ty> >
						(start, end, (start < Vec.size()) ? Vec[start] : _Identity_Element);
					return;
				}

				//internal node (non-leaf)
				int mid = (start + end) / 2;


				//construct this node with initial val(_Identity_Element)
				ST[index] = make_shared<SegmentTreeNode<_Ty> >(start, end, _Identity_Element);


				//construct left and right subTree (recursion)
				build((index << 1) + 1, start, mid, Vec);
				build((index << 1) + 2, mid + 1, end, Vec);


				//set value
				ST[index]->setValue(
					_STD move
					(_Func(ST[(index << 1) + 1]->value(), ST[(index << 1) + 2]->value())));

			}


			//suppose that this level's value is satisfied, and cache\aug is set appropriate.
			//then push down the aug/cache to the next level.
			//such that the next level's value is satisfied, and cache\aug is set appropriate. 
			//after pushDown, the current level's cache & augment has been null.
			//@ Parameter list:
			//@		const int index: the current node num
			//@ Promise: set the next level appropriately.(including value/cache/aug)
			void pushDown(const int index)
			{


				//length of the sub-interval
				const int sub_interval_length =
					(ST[index]->end() - ST[index]->start() + 1) / 2;


				//if the value_cache store the value of the range,
				//then set the next 2 child appropriate.
				if (value_cache[index].second)
				{

					//eliminate augment value
					aug[index] = _Identity_Element;


					//adjust the cache
					const _Ty value = value_cache[index].first;

					//update the cache
					value_cache[index] = _STD make_pair(_Identity_Element, false);

					_Ty temp = _Identity_Element;
					for (int i = 0; i < sub_interval_length; ++i)
						temp = _STD move(_Func(temp, value));
					_Ty temp2{ temp };
					ST[(index << 1) + 1]->setValue(_STD move(temp));
					ST[(index << 1) + 2]->setValue(_STD move(temp2));

					//update the next level if necessary
					if ((index << 1) + 1 < _size - 1)
					{
						value_cache[(index << 1) + 1] = _STD make_pair(value, true);
						value_cache[(index << 1) + 2] = _STD make_pair(value, true);
						aug[(index << 1) + 1] = _Identity_Element;
						aug[(index << 1) + 2] = _Identity_Element;
					}

					return;

				}



				//no cache value in this level
				//adjust the augment
				if (!(aug[index] == _Identity_Element))
				{

					const _Ty _value = aug[index];

					aug[index] = _Identity_Element;			//augmentation update


					//accumulate funciton to update the the interval value of the next level.
					std::tr1::function<void(SegmentTreeNode_ptr&)> _func_accumulate =
						[this, sub_interval_length, &_value](SegmentTreeNode_ptr& STNode_ptr)
					{
						_Ty value = STNode_ptr->val();

						for (int i = 0; i < sub_interval_length; ++i)
						{
							value = _STD move(_Func(value, _value));
						}

						STNode_ptr->setValue(_STD move(value));
					};


					//if index is not at the bottom level of the augmentation field,
					//then update the augmentation field of the next level.
					if ((index << 1) + 1 < _size - 1)
					{

						auto _func = [this, &_value](_Ty& value)->void
						{
							value = _STD move(_Func(value, _value));
						};


						auto _func_cache =
							[this, sub_interval_length]
						(SegmentTreeNode_ptr& STNode_ptr, const _Ty& cache_value)
						{
							_Ty temp = _Identity_Element;

							for (int i = 0; i < sub_interval_length; ++i)
								temp = _STD move(_Func(cache_value, temp));

							STNode_ptr->setValue(_STD move(temp));
						};


						bool left_child_has_cache = value_cache[(index << 1) + 1].second;
						bool right_child_has_cache = value_cache[(index << 1) + 2].second;

						//if next level has cache value,
						//	then update the cache by augment.
						//if not has cache,
						//	then update the augment.
						//finally update next level's value.

						if (left_child_has_cache)
						{
							_func(value_cache[(index << 1) + 1].first);
							aug[(index << 1) + 1] = _Identity_Element;
							_func_cache(ST[(index << 1) + 1], value_cache[(index << 1) + 1].first);
						}
						else
						{
							_func(aug[(index << 1) + 1]);
							_func_accumulate(ST[(index << 1) + 1]);
						}


						if (right_child_has_cache)
						{
							_func(value_cache[(index << 1) + 2].first);
							aug[(index << 1) + 2] = _Identity_Element;
							_func_cache(ST[(index << 1) + 2], value_cache[(index << 1) + 2].first);
						}
						else
						{
							_func(aug[(index << 1) + 2]);
							_func_accumulate(ST[(index << 1) + 2]);
						}

						return;

					}


					//has augment, and at the bottom in the augmentaiton field
					else
					{
						_func_accumulate(ST[(index << 1) + 1]);		//value update
						_func_accumulate(ST[(index << 1) + 2]);		//value update
					}


				}


			}//end function	pushDown(int)


			//adjust the index node.
			/****************************************************************\
			* usage: when you modify one level with augment, call adjust().  *
			\****************************************************************/
			//@ Parameter list:
			//@		const int index: the interval num
			//@		const _Ty& aug_value: augment
			//@ Promise: adjust current node with augment
			void adjust(const int index, const _Ty& aug_value)
			{

				//if cache stores some value
				if (index < _size - 1)
				{

					const int _length = ST[index]->end() - ST[index]->start() + 1;

					if (value_cache[index].second)
					{

						//eliminate augment_value
						aug[index] = _Identity_Element;


						//adjust the cache
						value_cache[index] = _STD make_pair
						((_Func(value_cache[index].first, aug_value)), true);


						//set the value
						const _Ty value = value_cache[index].first;

						_Ty temp = _Identity_Element;
						for (int i = 0; i < _length; ++i)
							temp = _STD move(_Func(temp, value));

						ST[index]->setValue(_STD move(temp));

					}


					//no cache
					//update aug[] and ST[]value.
					else {

						aug[index] = _STD move(_Func(aug[index], aug_value));

						_Ty temp = ST[index]->value();
						for (int i = 0; i < _length; ++i)
							temp = _STD move(_Func(temp, aug_value));

						ST[index]->setValue(_STD move(temp));

					}

				}//at bottom

			}//end function adjust(int, const _Ty)


			//querying the total value interval [start, end] applied by _Func
			//suppose the index for ST[], aug[], cache[] has been set appropriate.
			//@	Parameter list:
			//@		int index:	current node
			//@		int start:	start of querying rang
			//@		int end:	end of querying range
			_Ty doQuery(int index, int start, int end)
			{

				int left = ST[index]->start();
				int right = ST[index]->end();

				//no segment union
				if (start > right || end < left)
					return _Identity_Element;


				//querying segment includes root segment
				if (start <= left && right <= end)
				{

					//adjust(index, _Identity_Element);

					return ST[index]->value();

				}


				//update to the next level
				pushDown(index);


				//partially coincide
				return _Func(doQuery((index << 1) + 1, start, end),
					doQuery((index << 1) + 2, start, end));

			}


			//const member function
			_Ty doQuery(int index, int start, int end) const
			{

				//no segment union
				if (start > ST[index]->end() || end < ST[index]->start())
					return _Identity_Element;


				//querying segment includes root segment
				if (start <= ST[index]->start() && ST[index]->end() <= end)
					return ST[index]->val();


				//partially coincide
				return _Func(doQuery((index << 1) + 1, start, end),
					doQuery((index << 1) + 2, start, end));

			}



			/**********************************************************\
			* modify the range with substitution of the specific value *
			\**********************************************************/
			//@ Parameter list:
			//@		index: current node
			//@		start: start of the modifying rang
			//@		end:   end of the modifying range
			//@		value: the value to which the range wanted to be changed
			void doModify(const int index, const int start, const int end, const _Ty& value)
			{

				int left = ST[index]->start();
				int right = ST[index]->end();

				//no intersection
				if (start > right || end < left)return;

				const int _length = right - left + 1;

				//the current range is covered within the modifying range. 
				if (start <= left && end >= right)
				{

					//update cache and augment
					if (index < _size - 1)
					{
						value_cache[index] = _STD make_pair(value, true);
						aug[index] = _Identity_Element;
					}

					_Ty temp = _Identity_Element;
					for (int i = 0; i < _length; ++i)
						temp = _STD move(_Func(temp, value));

					ST[index]->setValue(_STD move(temp));

				}


				//some intersecton
				//so update to the next level
				else {

					pushDown(index);

					doModify((index << 1) + 1, start, end, value);
					doModify((index << 1) + 2, start, end, value);

					ST[index]->setValue(
						_STD move
						(_Func(ST[(index << 1) + 1]->value(), ST[(index << 1) + 2]->value())));

				}

			}//end function doModify


			//range updating with a augment value
			//@ Parameter list:
			//@		const int index:	the index of the interval
			//@		const int start:	start of modifing intereval
			//@		const int end:		end of modifying interval
			//@		const _Ty& value:	modifying value
			void doModify_augment
			(const int index, const int start, const int end, const _Ty& aug_value)
			{

				int left = ST[index]->start();
				int right = ST[index]->end();

				//no intersection
				if (start > right || end < left)return;

				//the current range is covered within the modifying range. 
				if (start <= left && end >= right)
				{

					//set the augmentation field
					if (index < _size - 1)
						adjust(index, aug_value);
					//aug[index] = _STD move(_Func(aug[index], aug_value));


					else
						ST[index]->setValue(
							_STD move
							(_Func(ST[index]->value(), aug_value)));


				}


				//some intersecton
				//so update to the next level
				else {

					pushDown(index);

					doModify_augment((index << 1) + 1, start, end, aug_value);
					doModify_augment((index << 1) + 2, start, end, aug_value);

					ST[index]->setValue(
						_STD move
						(_Func(ST[(index << 1) + 1]->value(), ST[(index << 1) + 2]->value())));

				}


			}



			/*************************************************\
			*@ specialization for different type of function  *
			\*************************************************/
			/********************************************************************\
			*@ Promise: func type should be NoHomeomorphism against the type _Ty *
			\********************************************************************/
			//ranging updating with modifying function
			//@ Parameter list:
			//@
			//@
			//@
			//@		modify_func:	NoHomeomorphism !!!
			void doModify(const int index, const int start, const int end,
				const modify_func& func, NoHomeomorphism = {})
			{

				//since the function is NoCommutive,
				//so we have to modify each element respectively


				/********************\
				* such stupid method *
				\********************/
				for (int i = start; i <= end; ++i)
					doModify(0, i, i, func(doQuery(0, i, i)));


			}



			/*************************************************\
			*@ specialization for different type of function  *
			\*************************************************/
			/******************************************************************\
			*@ Promise: func type should be Homeomorphism against the type _Ty *
			\******************************************************************/
			//ranging updating with modifying function
			//@ Parameter list:
			//@
			//@
			//@
			//@		modify_func:	Homeomorphism !!!!
			void doModify(const int index, const int start, const int end,
				const modify_func& func, Homeomorphism)
			{

				int left = ST[index]->start();
				int right = ST[index]->end();

				//no intersection
				if (start > right || end < left)return;


				//the current range is covered within the modifying range. 
				if (start <= left && end >= right)
				{

					//update augmentation field
					if (index < _size - 1) {}


					return;

				}




				//some intersecton
				//so update to the next level
				else {




				}

			}


		};

	}

}

#endif // !_SEGMENTTREEIMPL_H
