#include <iostream>
#include <map>
#include <vector>

/*
 * reference dyno on Github: https://github.com/ldionne/dyno
 */
namespace Prototype
{

	/*
	 * Use traditional inheritance and subtyping polymorphism tactic.
	 * flexible, but intrusive.
	 */
	namespace Inheritance
	{

		class Animal
		{
		public:
			virtual	Animal* clone() = 0;
			virtual void eat() = 0;
			virtual ~Animal() = 0 {};
		};

		class Dog :public Animal
		{
		public:
			Animal * clone() { return new Dog(); }
			void eat() { std::cout << "Dog eating." << std::endl; }
		};

		class Cat :public Animal
		{
		public:
			Animal * clone() { return new Cat(); }
			void eat() { std::cout << "Cat eating." << std::endl; }
		};


		void test()
		{
			using id_t = int;
			std::map<id_t, Animal*> animal_map;
			animal_map.emplace(233, new Dog());
			animal_map.emplace(7, new Cat());

			std::vector<Animal*> animal_container; // store the data in another container.
			for (auto animal : animal_map)
			{
				animal.second->eat();
				animal_container.push_back(animal.second->clone());
			}

			// free memory
			for (auto animal : animal_container)
				delete animal;
			for (auto animal : animal_map)
				delete animal.second;

		}

	}


	/*
	 * Use Parametric polymorphism tactic.
	 * flexible, and non-intrusive.
	 */
	namespace No_Inheritance
	{

		struct _Animal_VTable
		{
			void(*eat)(void* this_);
			void* (*clone_)(void** this_, void* other);
			void(*delete_)(void* this_);
		};

		template<typename _Animal_t> _Animal_VTable constexpr vtable_for =
			_Animal_VTable
		{
			// eat
			[](void* this_)->decltype(auto) {(static_cast<_Animal_t*>(this_))->eat(); },

			// clone
			[](void** this_, void* other)->void* {
			if constexpr(sizeof(_Animal_t) <= Animal::buffer_size)
				return new (static_cast<void*>(this_ + 1U))
				_Animal_t{ *static_cast<_Animal_t*>(other) };
			else return new _Animal_t{ *static_cast<_Animal_t*>(other) }; },

			// delete
				[](void* this_) {
				if (this_ == *static_cast<void**>(static_cast<void**>(this_) - 1U))
					static_cast<_Animal_t*>(this_)->~_Animal_t();
				else delete static_cast<_Animal_t*>(this_); },

		};

		class Animal
		{
			static constexpr std::size_t buffer_size = 16U;
		public:
			template<typename _Animal_t>
			Animal(const _Animal_t& animal)
				:vptr_{ &vtable_for<_Animal_t> },
				_animal(nullptr)
			{
				if constexpr(sizeof(_Animal_t) <= buffer_size)
					_animal = new (buffer)_Animal_t{ animal };
				else _animal = new _Animal_t{ animal };
			}

			Animal(const Animal& other) :vptr_(other.vptr_), _animal(vptr_->clone_(&_animal, other._animal)) {}

			Animal& operator=(const Animal& other)
			{
				if (this == &other) return *this;
				vptr_->delete_(_animal);
				vptr_ = other.vptr_;
				_animal = vptr_->clone_(&_animal, other._animal);
				return *this;
			}

			Animal(Animal&& other) = delete;

			Animal& operator=(Animal&& other) = delete;

			void eat() { vptr_->eat(_animal); }

			void swap(Animal& other)
			{
				std::swap(vptr_, other.vptr_);
				std::swap(_animal, other._animal);
				char temp_buffer[Animal::buffer_size];
				std::memcpy(temp_buffer, buffer, Animal::buffer_size);
				std::memcpy(buffer, other.buffer, Animal::buffer_size);
				std::memcpy(other.buffer, temp_buffer, Animal::buffer_size);
			}

			~Animal() { vptr_->delete_(_animal); }

		private:
			_Animal_VTable const* vptr_;
			void * _animal;
			char buffer[buffer_size]; // SBO
		};

		class Dog {
		public:
			void eat() { std::cout << "Dog eating." << std::endl; }
		};
		class Cat {
		public:
			void eat() { std::cout << "Cat eating." << std::endl; }
		};

		void test()
		{
			using id_t = int;
			std::map<id_t, Animal> animal_map;
			Cat c;
			Dog d;
			animal_map.insert(std::make_pair(233, c));
			animal_map.insert(std::make_pair(77, d));

			std::vector<Animal> animal_container;
			for (auto animal : animal_map)
			{
				animal.second.eat();
				//animal_container.push_back(animal.second); // WTF??? invoke move ctor
			}


			Animal a{ Cat{} };
			Animal b = a;
			Animal d2{ Dog{} };
			a = d2;
			a.eat();
			b.eat();
			d2.eat();
		}

	}

}
