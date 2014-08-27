// This file was autogenerated by cloop - Cross Language Object Oriented Programming

#ifndef CALC_CPP_API_H
#define CALC_CPP_API_H

#include <stdint.h>


template <typename Policy>
class CalcApi
{
public:
	// Interfaces declarations

	class Disposable
	{
	protected:
		struct VTable
		{
			void* cloopDummy[1];
			uintptr_t version;
			void (*dispose)(Disposable* self);
		};

	protected:
		void* cloopDummy[1];
		VTable* cloopVTable;

	public:
		static const int VERSION = 1;

		void dispose()
		{
			Policy::template checkVersion<1>(this);
			static_cast<VTable*>(this->cloopVTable)->dispose(this);
		}
	};

	class Status : public Disposable
	{
	protected:
		struct VTable : public Disposable::VTable
		{
			int (*getCode)(const Status* self);
			void (*setCode)(Status* self, int code);
		};

	public:
		static const int VERSION = 3;

		static const int ERROR_1 = 1;
		static const int ERROR_2 = 2;

		int getCode() const
		{
			Policy::template checkVersion<2>(this);
			int ret = static_cast<VTable*>(this->cloopVTable)->getCode(this);
			return ret;
		}

		void setCode(int code)
		{
			Policy::template checkVersion<3>(this);
			static_cast<VTable*>(this->cloopVTable)->setCode(this, code);
		}
	};

	class Calculator : public Disposable
	{
	protected:
		struct VTable : public Disposable::VTable
		{
			int (*sum)(const Calculator* self, Status* status, int n1, int n2);
			int (*getMemory)(const Calculator* self);
			void (*setMemory)(Calculator* self, int n);
			void (*sumAndStore)(Calculator* self, Status* status, int n1, int n2);
		};

	public:
		static const int VERSION = 5;

		int sum(Status* status, int n1, int n2) const
		{
			Policy::template checkVersion<2>(this);
			int ret = static_cast<VTable*>(this->cloopVTable)->sum(this, status, n1, n2);
			Policy::checkException(status);
			return ret;
		}

		int getMemory() const
		{
			Policy::template checkVersion<3>(this);
			int ret = static_cast<VTable*>(this->cloopVTable)->getMemory(this);
			return ret;
		}

		void setMemory(int n)
		{
			Policy::template checkVersion<4>(this);
			static_cast<VTable*>(this->cloopVTable)->setMemory(this, n);
		}

		void sumAndStore(Status* status, int n1, int n2)
		{
			Policy::template checkVersion<5>(this);
			static_cast<VTable*>(this->cloopVTable)->sumAndStore(this, status, n1, n2);
			Policy::checkException(status);
		}
	};

	class Calculator2 : public Calculator
	{
	protected:
		struct VTable : public Calculator::VTable
		{
			int (*multiply)(const Calculator2* self, Status* status, int n1, int n2);
			void (*copyMemory)(Calculator2* self, const Calculator* calculator);
			void (*copyMemory2)(Calculator2* self, const int* address);
		};

	public:
		static const int VERSION = 8;

		int multiply(Status* status, int n1, int n2) const
		{
			Policy::template checkVersion<5>(this);
			int ret = static_cast<VTable*>(this->cloopVTable)->multiply(this, status, n1, n2);
			Policy::checkException(status);
			return ret;
		}

		void copyMemory(const Calculator* calculator)
		{
			Policy::template checkVersion<6>(this);
			static_cast<VTable*>(this->cloopVTable)->copyMemory(this, calculator);
		}

		void copyMemory2(const int* address)
		{
			Policy::template checkVersion<7>(this);
			static_cast<VTable*>(this->cloopVTable)->copyMemory2(this, address);
		}
	};

	class Factory : public Disposable
	{
	protected:
		struct VTable : public Disposable::VTable
		{
			Status* (*createStatus)(Factory* self);
			Calculator* (*createCalculator)(Factory* self, Status* status);
			Calculator2* (*createCalculator2)(Factory* self, Status* status);
			Calculator* (*createBrokenCalculator)(Factory* self, Status* status);
		};

	public:
		static const int VERSION = 5;

		Status* createStatus()
		{
			Policy::template checkVersion<2>(this);
			Status* ret = static_cast<VTable*>(this->cloopVTable)->createStatus(this);
			return ret;
		}

		Calculator* createCalculator(Status* status)
		{
			Policy::template checkVersion<3>(this);
			Calculator* ret = static_cast<VTable*>(this->cloopVTable)->createCalculator(this, status);
			Policy::checkException(status);
			return ret;
		}

		Calculator2* createCalculator2(Status* status)
		{
			Policy::template checkVersion<4>(this);
			Calculator2* ret = static_cast<VTable*>(this->cloopVTable)->createCalculator2(this, status);
			Policy::checkException(status);
			return ret;
		}

		Calculator* createBrokenCalculator(Status* status)
		{
			Policy::template checkVersion<5>(this);
			Calculator* ret = static_cast<VTable*>(this->cloopVTable)->createBrokenCalculator(this, status);
			Policy::checkException(status);
			return ret;
		}
	};

	// Interfaces implementations

	template <typename Name, typename Base>
	class DisposableBaseImpl : public Base
	{
	public:
		DisposableBaseImpl()
		{
			static struct VTableImpl : Base::VTable
			{
				VTableImpl()
				{
					this->version = Base::VERSION;
					this->dispose = &Name::cloopdisposeDispatcher;
				}
			} vTable;

			this->cloopVTable = &vTable;
		}

		static void cloopdisposeDispatcher(Disposable* self) throw()
		{
			try
			{
				static_cast<Name*>(self)->Name::dispose();
			}
			catch (...)
			{
				Policy::catchException(0);
			}
		}
	};

	template <typename Name, typename Base = Disposable>
	class DisposableImpl : public DisposableBaseImpl<Name, Base>
	{
	public:
		virtual ~DisposableImpl()
		{
		}

		virtual void dispose() = 0;
	};

	template <typename Name, typename Base>
	class StatusBaseImpl : public Base
	{
	public:
		StatusBaseImpl()
		{
			static struct VTableImpl : Base::VTable
			{
				VTableImpl()
				{
					this->version = Base::VERSION;
					this->dispose = &Name::cloopdisposeDispatcher;
					this->getCode = &Name::cloopgetCodeDispatcher;
					this->setCode = &Name::cloopsetCodeDispatcher;
				}
			} vTable;

			this->cloopVTable = &vTable;
		}

		static int cloopgetCodeDispatcher(const Status* self) throw()
		{
			try
			{
				return static_cast<const Name*>(self)->Name::getCode();
			}
			catch (...)
			{
				Policy::catchException(0);
				return static_cast<int>(0);
			}
		}

		static void cloopsetCodeDispatcher(Status* self, int code) throw()
		{
			try
			{
				static_cast<Name*>(self)->Name::setCode(code);
			}
			catch (...)
			{
				Policy::catchException(0);
			}
		}

		static void cloopdisposeDispatcher(Disposable* self) throw()
		{
			try
			{
				static_cast<Name*>(self)->Name::dispose();
			}
			catch (...)
			{
				Policy::catchException(0);
			}
		}
	};

	template <typename Name, typename Base = DisposableImpl<Name, Status> >
	class StatusImpl : public StatusBaseImpl<Name, Base>
	{
	public:
		virtual ~StatusImpl()
		{
		}

		virtual int getCode() const = 0;
		virtual void setCode(int code) = 0;
	};

	template <typename Name, typename Base>
	class CalculatorBaseImpl : public Base
	{
	public:
		CalculatorBaseImpl()
		{
			static struct VTableImpl : Base::VTable
			{
				VTableImpl()
				{
					this->version = Base::VERSION;
					this->dispose = &Name::cloopdisposeDispatcher;
					this->sum = &Name::cloopsumDispatcher;
					this->getMemory = &Name::cloopgetMemoryDispatcher;
					this->setMemory = &Name::cloopsetMemoryDispatcher;
					this->sumAndStore = &Name::cloopsumAndStoreDispatcher;
				}
			} vTable;

			this->cloopVTable = &vTable;
		}

		static int cloopsumDispatcher(const Calculator* self, Status* status, int n1, int n2) throw()
		{
			try
			{
				return static_cast<const Name*>(self)->Name::sum(status, n1, n2);
			}
			catch (...)
			{
				Policy::catchException(status);
				return static_cast<int>(0);
			}
		}

		static int cloopgetMemoryDispatcher(const Calculator* self) throw()
		{
			try
			{
				return static_cast<const Name*>(self)->Name::getMemory();
			}
			catch (...)
			{
				Policy::catchException(0);
				return static_cast<int>(0);
			}
		}

		static void cloopsetMemoryDispatcher(Calculator* self, int n) throw()
		{
			try
			{
				static_cast<Name*>(self)->Name::setMemory(n);
			}
			catch (...)
			{
				Policy::catchException(0);
			}
		}

		static void cloopsumAndStoreDispatcher(Calculator* self, Status* status, int n1, int n2) throw()
		{
			try
			{
				static_cast<Name*>(self)->Name::sumAndStore(status, n1, n2);
			}
			catch (...)
			{
				Policy::catchException(status);
			}
		}

		static void cloopdisposeDispatcher(Disposable* self) throw()
		{
			try
			{
				static_cast<Name*>(self)->Name::dispose();
			}
			catch (...)
			{
				Policy::catchException(0);
			}
		}
	};

	template <typename Name, typename Base = DisposableImpl<Name, Calculator> >
	class CalculatorImpl : public CalculatorBaseImpl<Name, Base>
	{
	public:
		virtual ~CalculatorImpl()
		{
		}

		virtual int sum(Status* status, int n1, int n2) const = 0;
		virtual int getMemory() const = 0;
		virtual void setMemory(int n) = 0;
		virtual void sumAndStore(Status* status, int n1, int n2) = 0;
	};

	template <typename Name, typename Base>
	class Calculator2BaseImpl : public Base
	{
	public:
		Calculator2BaseImpl()
		{
			static struct VTableImpl : Base::VTable
			{
				VTableImpl()
				{
					this->version = Base::VERSION;
					this->dispose = &Name::cloopdisposeDispatcher;
					this->sum = &Name::cloopsumDispatcher;
					this->getMemory = &Name::cloopgetMemoryDispatcher;
					this->setMemory = &Name::cloopsetMemoryDispatcher;
					this->sumAndStore = &Name::cloopsumAndStoreDispatcher;
					this->multiply = &Name::cloopmultiplyDispatcher;
					this->copyMemory = &Name::cloopcopyMemoryDispatcher;
					this->copyMemory2 = &Name::cloopcopyMemory2Dispatcher;
				}
			} vTable;

			this->cloopVTable = &vTable;
		}

		static int cloopmultiplyDispatcher(const Calculator2* self, Status* status, int n1, int n2) throw()
		{
			try
			{
				return static_cast<const Name*>(self)->Name::multiply(status, n1, n2);
			}
			catch (...)
			{
				Policy::catchException(status);
				return static_cast<int>(0);
			}
		}

		static void cloopcopyMemoryDispatcher(Calculator2* self, const Calculator* calculator) throw()
		{
			try
			{
				static_cast<Name*>(self)->Name::copyMemory(calculator);
			}
			catch (...)
			{
				Policy::catchException(0);
			}
		}

		static void cloopcopyMemory2Dispatcher(Calculator2* self, const int* address) throw()
		{
			try
			{
				static_cast<Name*>(self)->Name::copyMemory2(address);
			}
			catch (...)
			{
				Policy::catchException(0);
			}
		}

		static int cloopsumDispatcher(const Calculator* self, Status* status, int n1, int n2) throw()
		{
			try
			{
				return static_cast<const Name*>(self)->Name::sum(status, n1, n2);
			}
			catch (...)
			{
				Policy::catchException(status);
				return static_cast<int>(0);
			}
		}

		static int cloopgetMemoryDispatcher(const Calculator* self) throw()
		{
			try
			{
				return static_cast<const Name*>(self)->Name::getMemory();
			}
			catch (...)
			{
				Policy::catchException(0);
				return static_cast<int>(0);
			}
		}

		static void cloopsetMemoryDispatcher(Calculator* self, int n) throw()
		{
			try
			{
				static_cast<Name*>(self)->Name::setMemory(n);
			}
			catch (...)
			{
				Policy::catchException(0);
			}
		}

		static void cloopsumAndStoreDispatcher(Calculator* self, Status* status, int n1, int n2) throw()
		{
			try
			{
				static_cast<Name*>(self)->Name::sumAndStore(status, n1, n2);
			}
			catch (...)
			{
				Policy::catchException(status);
			}
		}

		static void cloopdisposeDispatcher(Disposable* self) throw()
		{
			try
			{
				static_cast<Name*>(self)->Name::dispose();
			}
			catch (...)
			{
				Policy::catchException(0);
			}
		}
	};

	template <typename Name, typename Base = CalculatorImpl<Name, DisposableImpl<Name, Calculator2> > >
	class Calculator2Impl : public Calculator2BaseImpl<Name, Base>
	{
	public:
		virtual ~Calculator2Impl()
		{
		}

		virtual int multiply(Status* status, int n1, int n2) const = 0;
		virtual void copyMemory(const Calculator* calculator) = 0;
		virtual void copyMemory2(const int* address) = 0;
	};

	template <typename Name, typename Base>
	class FactoryBaseImpl : public Base
	{
	public:
		FactoryBaseImpl()
		{
			static struct VTableImpl : Base::VTable
			{
				VTableImpl()
				{
					this->version = Base::VERSION;
					this->dispose = &Name::cloopdisposeDispatcher;
					this->createStatus = &Name::cloopcreateStatusDispatcher;
					this->createCalculator = &Name::cloopcreateCalculatorDispatcher;
					this->createCalculator2 = &Name::cloopcreateCalculator2Dispatcher;
					this->createBrokenCalculator = &Name::cloopcreateBrokenCalculatorDispatcher;
				}
			} vTable;

			this->cloopVTable = &vTable;
		}

		static Status* cloopcreateStatusDispatcher(Factory* self) throw()
		{
			try
			{
				return static_cast<Name*>(self)->Name::createStatus();
			}
			catch (...)
			{
				Policy::catchException(0);
				return static_cast<Status*>(0);
			}
		}

		static Calculator* cloopcreateCalculatorDispatcher(Factory* self, Status* status) throw()
		{
			try
			{
				return static_cast<Name*>(self)->Name::createCalculator(status);
			}
			catch (...)
			{
				Policy::catchException(status);
				return static_cast<Calculator*>(0);
			}
		}

		static Calculator2* cloopcreateCalculator2Dispatcher(Factory* self, Status* status) throw()
		{
			try
			{
				return static_cast<Name*>(self)->Name::createCalculator2(status);
			}
			catch (...)
			{
				Policy::catchException(status);
				return static_cast<Calculator2*>(0);
			}
		}

		static Calculator* cloopcreateBrokenCalculatorDispatcher(Factory* self, Status* status) throw()
		{
			try
			{
				return static_cast<Name*>(self)->Name::createBrokenCalculator(status);
			}
			catch (...)
			{
				Policy::catchException(status);
				return static_cast<Calculator*>(0);
			}
		}

		static void cloopdisposeDispatcher(Disposable* self) throw()
		{
			try
			{
				static_cast<Name*>(self)->Name::dispose();
			}
			catch (...)
			{
				Policy::catchException(0);
			}
		}
	};

	template <typename Name, typename Base = DisposableImpl<Name, Factory> >
	class FactoryImpl : public FactoryBaseImpl<Name, Base>
	{
	public:
		virtual ~FactoryImpl()
		{
		}

		virtual Status* createStatus() = 0;
		virtual Calculator* createCalculator(Status* status) = 0;
		virtual Calculator2* createCalculator2(Status* status) = 0;
		virtual Calculator* createBrokenCalculator(Status* status) = 0;
	};
};


#endif	// CALC_CPP_API_H
