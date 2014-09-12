{ This file was autogenerated by cloop - Cross Language Object Oriented Programming }

unit CalcPascalApi;

interface

uses Classes, SysUtils;

type
	Disposable = class;
	Status = class;
	Factory = class;
	Calculator = class;
	Calculator2 = class;

	IntegerPtr = ^Integer;

	Disposable_disposePtr = procedure(this: Disposable); cdecl;
	Status_getCodePtr = function(this: Status): Integer; cdecl;
	Status_setCodePtr = procedure(this: Status; code: Integer); cdecl;
	Factory_createStatusPtr = function(this: Factory): Status; cdecl;
	Factory_createCalculatorPtr = function(this: Factory; status: Status): Calculator; cdecl;
	Factory_createCalculator2Ptr = function(this: Factory; status: Status): Calculator2; cdecl;
	Factory_createBrokenCalculatorPtr = function(this: Factory; status: Status): Calculator; cdecl;
	Calculator_sumPtr = function(this: Calculator; status: Status; n1: Integer; n2: Integer): Integer; cdecl;
	Calculator_getMemoryPtr = function(this: Calculator): Integer; cdecl;
	Calculator_setMemoryPtr = procedure(this: Calculator; n: Integer); cdecl;
	Calculator_sumAndStorePtr = procedure(this: Calculator; status: Status; n1: Integer; n2: Integer); cdecl;
	Calculator2_multiplyPtr = function(this: Calculator2; status: Status; n1: Integer; n2: Integer): Integer; cdecl;
	Calculator2_copyMemoryPtr = procedure(this: Calculator2; calculator: Calculator); cdecl;
	Calculator2_copyMemory2Ptr = procedure(this: Calculator2; address: IntegerPtr); cdecl;

	DisposableVTable = class
{$ifndef FPC}
		dummy: NativeInt;
{$endif}
		version: NativeInt;
		dispose: Disposable_disposePtr;
	end;

	Disposable = class
{$ifndef FPC}
		dummy: NativeInt;
{$endif}
		vTable: DisposableVTable;
		procedure dispose();
	end;

	DisposableImpl = class(Disposable)
		constructor create;

		procedure dispose(); virtual; abstract;
	end;

	StatusVTable = class(DisposableVTable)
		getCode: Status_getCodePtr;
		setCode: Status_setCodePtr;
	end;

	Status = class(Disposable)
		function getCode(): Integer;
		procedure setCode(code: Integer);
	end;

	StatusImpl = class(Status)
		constructor create;

		procedure dispose(); virtual; abstract;
		function getCode(): Integer; virtual; abstract;
		procedure setCode(code: Integer); virtual; abstract;
	end;

	FactoryVTable = class(DisposableVTable)
		createStatus: Factory_createStatusPtr;
		createCalculator: Factory_createCalculatorPtr;
		createCalculator2: Factory_createCalculator2Ptr;
		createBrokenCalculator: Factory_createBrokenCalculatorPtr;
	end;

	Factory = class(Disposable)
		function createStatus(): Status;
		function createCalculator(status: Status): Calculator;
		function createCalculator2(status: Status): Calculator2;
		function createBrokenCalculator(status: Status): Calculator;
	end;

	FactoryImpl = class(Factory)
		constructor create;

		procedure dispose(); virtual; abstract;
		function createStatus(): Status; virtual; abstract;
		function createCalculator(status: Status): Calculator; virtual; abstract;
		function createCalculator2(status: Status): Calculator2; virtual; abstract;
		function createBrokenCalculator(status: Status): Calculator; virtual; abstract;
	end;

	CalculatorVTable = class(DisposableVTable)
		sum: Calculator_sumPtr;
		getMemory: Calculator_getMemoryPtr;
		setMemory: Calculator_setMemoryPtr;
		sumAndStore: Calculator_sumAndStorePtr;
	end;

	Calculator = class(Disposable)
		function sum(status: Status; n1: Integer; n2: Integer): Integer;
		function getMemory(): Integer;
		procedure setMemory(n: Integer);
		procedure sumAndStore(status: Status; n1: Integer; n2: Integer);
	end;

	CalculatorImpl = class(Calculator)
		constructor create;

		procedure dispose(); virtual; abstract;
		function sum(status: Status; n1: Integer; n2: Integer): Integer; virtual; abstract;
		function getMemory(): Integer; virtual; abstract;
		procedure setMemory(n: Integer); virtual; abstract;
		procedure sumAndStore(status: Status; n1: Integer; n2: Integer); virtual; abstract;
	end;

	Calculator2VTable = class(CalculatorVTable)
		multiply: Calculator2_multiplyPtr;
		copyMemory: Calculator2_copyMemoryPtr;
		copyMemory2: Calculator2_copyMemory2Ptr;
	end;

	Calculator2 = class(Calculator)
		function multiply(status: Status; n1: Integer; n2: Integer): Integer;
		procedure copyMemory(calculator: Calculator);
		procedure copyMemory2(address: IntegerPtr);
	end;

	Calculator2Impl = class(Calculator2)
		constructor create;

		procedure dispose(); virtual; abstract;
		function sum(status: Status; n1: Integer; n2: Integer): Integer; virtual; abstract;
		function getMemory(): Integer; virtual; abstract;
		procedure setMemory(n: Integer); virtual; abstract;
		procedure sumAndStore(status: Status; n1: Integer; n2: Integer); virtual; abstract;
		function multiply(status: Status; n1: Integer; n2: Integer): Integer; virtual; abstract;
		procedure copyMemory(calculator: Calculator); virtual; abstract;
		procedure copyMemory2(address: IntegerPtr); virtual; abstract;
	end;

CalcException = class(Exception)
public
	constructor create(code: Integer);

	function getCode: Integer;

	class procedure checkException(status: Status);
	class procedure catchException(status: Status; e: Exception);

private
	code: Integer;
end;

implementation

procedure Disposable.dispose();
begin
	DisposableVTable(vTable).dispose(Self);
end;

function Status.getCode(): Integer;
begin
	Result := StatusVTable(vTable).getCode(Self);
end;

procedure Status.setCode(code: Integer);
begin
	StatusVTable(vTable).setCode(Self, code);
end;

function Factory.createStatus(): Status;
begin
	Result := FactoryVTable(vTable).createStatus(Self);
end;

function Factory.createCalculator(status: Status): Calculator;
begin
	Result := FactoryVTable(vTable).createCalculator(Self, status);
	CalcException.checkException(status);
end;

function Factory.createCalculator2(status: Status): Calculator2;
begin
	Result := FactoryVTable(vTable).createCalculator2(Self, status);
	CalcException.checkException(status);
end;

function Factory.createBrokenCalculator(status: Status): Calculator;
begin
	Result := FactoryVTable(vTable).createBrokenCalculator(Self, status);
	CalcException.checkException(status);
end;

function Calculator.sum(status: Status; n1: Integer; n2: Integer): Integer;
begin
	Result := CalculatorVTable(vTable).sum(Self, status, n1, n2);
	CalcException.checkException(status);
end;

function Calculator.getMemory(): Integer;
begin
	Result := CalculatorVTable(vTable).getMemory(Self);
end;

procedure Calculator.setMemory(n: Integer);
begin
	CalculatorVTable(vTable).setMemory(Self, n);
end;

procedure Calculator.sumAndStore(status: Status; n1: Integer; n2: Integer);
begin
	CalculatorVTable(vTable).sumAndStore(Self, status, n1, n2);
	CalcException.checkException(status);
end;

function Calculator2.multiply(status: Status; n1: Integer; n2: Integer): Integer;
begin
	Result := Calculator2VTable(vTable).multiply(Self, status, n1, n2);
	CalcException.checkException(status);
end;

procedure Calculator2.copyMemory(calculator: Calculator);
begin
	Calculator2VTable(vTable).copyMemory(Self, calculator);
end;

procedure Calculator2.copyMemory2(address: IntegerPtr);
begin
	Calculator2VTable(vTable).copyMemory2(Self, address);
end;

procedure DisposableImpl_disposeDispatcher(this: Disposable); cdecl;
begin
	try
		DisposableImpl(this).dispose();
	except
		on e: Exception do CalcException.catchException(nil, e);
	end
end;

var
	DisposableImpl_vTable: DisposableVTable;

constructor DisposableImpl.create;
begin
	vTable := DisposableImpl_vTable;
end;

procedure StatusImpl_disposeDispatcher(this: Status); cdecl;
begin
	try
		StatusImpl(this).dispose();
	except
		on e: Exception do CalcException.catchException(nil, e);
	end
end;

function StatusImpl_getCodeDispatcher(this: Status): Integer; cdecl;
begin
	try
		Result := StatusImpl(this).getCode();
	except
		on e: Exception do CalcException.catchException(nil, e);
	end
end;

procedure StatusImpl_setCodeDispatcher(this: Status; code: Integer); cdecl;
begin
	try
		StatusImpl(this).setCode(code);
	except
		on e: Exception do CalcException.catchException(nil, e);
	end
end;

var
	StatusImpl_vTable: StatusVTable;

constructor StatusImpl.create;
begin
	vTable := StatusImpl_vTable;
end;

procedure FactoryImpl_disposeDispatcher(this: Factory); cdecl;
begin
	try
		FactoryImpl(this).dispose();
	except
		on e: Exception do CalcException.catchException(nil, e);
	end
end;

function FactoryImpl_createStatusDispatcher(this: Factory): Status; cdecl;
begin
	try
		Result := FactoryImpl(this).createStatus();
	except
		on e: Exception do CalcException.catchException(nil, e);
	end
end;

function FactoryImpl_createCalculatorDispatcher(this: Factory; status: Status): Calculator; cdecl;
begin
	try
		Result := FactoryImpl(this).createCalculator(status);
	except
		on e: Exception do CalcException.catchException(status, e);
	end
end;

function FactoryImpl_createCalculator2Dispatcher(this: Factory; status: Status): Calculator2; cdecl;
begin
	try
		Result := FactoryImpl(this).createCalculator2(status);
	except
		on e: Exception do CalcException.catchException(status, e);
	end
end;

function FactoryImpl_createBrokenCalculatorDispatcher(this: Factory; status: Status): Calculator; cdecl;
begin
	try
		Result := FactoryImpl(this).createBrokenCalculator(status);
	except
		on e: Exception do CalcException.catchException(status, e);
	end
end;

var
	FactoryImpl_vTable: FactoryVTable;

constructor FactoryImpl.create;
begin
	vTable := FactoryImpl_vTable;
end;

procedure CalculatorImpl_disposeDispatcher(this: Calculator); cdecl;
begin
	try
		CalculatorImpl(this).dispose();
	except
		on e: Exception do CalcException.catchException(nil, e);
	end
end;

function CalculatorImpl_sumDispatcher(this: Calculator; status: Status; n1: Integer; n2: Integer): Integer; cdecl;
begin
	try
		Result := CalculatorImpl(this).sum(status, n1, n2);
	except
		on e: Exception do CalcException.catchException(status, e);
	end
end;

function CalculatorImpl_getMemoryDispatcher(this: Calculator): Integer; cdecl;
begin
	try
		Result := CalculatorImpl(this).getMemory();
	except
		on e: Exception do CalcException.catchException(nil, e);
	end
end;

procedure CalculatorImpl_setMemoryDispatcher(this: Calculator; n: Integer); cdecl;
begin
	try
		CalculatorImpl(this).setMemory(n);
	except
		on e: Exception do CalcException.catchException(nil, e);
	end
end;

procedure CalculatorImpl_sumAndStoreDispatcher(this: Calculator; status: Status; n1: Integer; n2: Integer); cdecl;
begin
	try
		CalculatorImpl(this).sumAndStore(status, n1, n2);
	except
		on e: Exception do CalcException.catchException(status, e);
	end
end;

var
	CalculatorImpl_vTable: CalculatorVTable;

constructor CalculatorImpl.create;
begin
	vTable := CalculatorImpl_vTable;
end;

procedure Calculator2Impl_disposeDispatcher(this: Calculator2); cdecl;
begin
	try
		Calculator2Impl(this).dispose();
	except
		on e: Exception do CalcException.catchException(nil, e);
	end
end;

function Calculator2Impl_sumDispatcher(this: Calculator2; status: Status; n1: Integer; n2: Integer): Integer; cdecl;
begin
	try
		Result := Calculator2Impl(this).sum(status, n1, n2);
	except
		on e: Exception do CalcException.catchException(status, e);
	end
end;

function Calculator2Impl_getMemoryDispatcher(this: Calculator2): Integer; cdecl;
begin
	try
		Result := Calculator2Impl(this).getMemory();
	except
		on e: Exception do CalcException.catchException(nil, e);
	end
end;

procedure Calculator2Impl_setMemoryDispatcher(this: Calculator2; n: Integer); cdecl;
begin
	try
		Calculator2Impl(this).setMemory(n);
	except
		on e: Exception do CalcException.catchException(nil, e);
	end
end;

procedure Calculator2Impl_sumAndStoreDispatcher(this: Calculator2; status: Status; n1: Integer; n2: Integer); cdecl;
begin
	try
		Calculator2Impl(this).sumAndStore(status, n1, n2);
	except
		on e: Exception do CalcException.catchException(status, e);
	end
end;

function Calculator2Impl_multiplyDispatcher(this: Calculator2; status: Status; n1: Integer; n2: Integer): Integer; cdecl;
begin
	try
		Result := Calculator2Impl(this).multiply(status, n1, n2);
	except
		on e: Exception do CalcException.catchException(status, e);
	end
end;

procedure Calculator2Impl_copyMemoryDispatcher(this: Calculator2; calculator: Calculator); cdecl;
begin
	try
		Calculator2Impl(this).copyMemory(calculator);
	except
		on e: Exception do CalcException.catchException(nil, e);
	end
end;

procedure Calculator2Impl_copyMemory2Dispatcher(this: Calculator2; address: IntegerPtr); cdecl;
begin
	try
		Calculator2Impl(this).copyMemory2(address);
	except
		on e: Exception do CalcException.catchException(nil, e);
	end
end;

var
	Calculator2Impl_vTable: Calculator2VTable;

constructor Calculator2Impl.create;
begin
	vTable := Calculator2Impl_vTable;
end;

constructor CalcException.create(code: Integer);
begin
	self.code := code;
end;

function CalcException.getCode: Integer;
begin
	Result := code;
end;

class procedure CalcException.checkException(status: Status);
var
	code: Integer;
begin
	code := status.getCode();

	if (code <> 0) then
		raise CalcException.create(code);
end;

class procedure CalcException.catchException(status: Status; e: Exception);
begin
	if (e.inheritsFrom(CalcException)) then
		status.setCode(CalcException(e).code)
	else
		status.setCode(-1);
end;
initialization
	DisposableImpl_vTable := DisposableVTable.create;
	DisposableImpl_vTable.version := 1;
	DisposableImpl_vTable.dispose := @DisposableImpl_disposeDispatcher;

	StatusImpl_vTable := StatusVTable.create;
	StatusImpl_vTable.version := 3;
	StatusImpl_vTable.dispose := @StatusImpl_disposeDispatcher;
	StatusImpl_vTable.getCode := @StatusImpl_getCodeDispatcher;
	StatusImpl_vTable.setCode := @StatusImpl_setCodeDispatcher;

	FactoryImpl_vTable := FactoryVTable.create;
	FactoryImpl_vTable.version := 5;
	FactoryImpl_vTable.dispose := @FactoryImpl_disposeDispatcher;
	FactoryImpl_vTable.createStatus := @FactoryImpl_createStatusDispatcher;
	FactoryImpl_vTable.createCalculator := @FactoryImpl_createCalculatorDispatcher;
	FactoryImpl_vTable.createCalculator2 := @FactoryImpl_createCalculator2Dispatcher;
	FactoryImpl_vTable.createBrokenCalculator := @FactoryImpl_createBrokenCalculatorDispatcher;

	CalculatorImpl_vTable := CalculatorVTable.create;
	CalculatorImpl_vTable.version := 5;
	CalculatorImpl_vTable.dispose := @CalculatorImpl_disposeDispatcher;
	CalculatorImpl_vTable.sum := @CalculatorImpl_sumDispatcher;
	CalculatorImpl_vTable.getMemory := @CalculatorImpl_getMemoryDispatcher;
	CalculatorImpl_vTable.setMemory := @CalculatorImpl_setMemoryDispatcher;
	CalculatorImpl_vTable.sumAndStore := @CalculatorImpl_sumAndStoreDispatcher;

	Calculator2Impl_vTable := Calculator2VTable.create;
	Calculator2Impl_vTable.version := 8;
	Calculator2Impl_vTable.dispose := @Calculator2Impl_disposeDispatcher;
	Calculator2Impl_vTable.sum := @Calculator2Impl_sumDispatcher;
	Calculator2Impl_vTable.getMemory := @Calculator2Impl_getMemoryDispatcher;
	Calculator2Impl_vTable.setMemory := @Calculator2Impl_setMemoryDispatcher;
	Calculator2Impl_vTable.sumAndStore := @Calculator2Impl_sumAndStoreDispatcher;
	Calculator2Impl_vTable.multiply := @Calculator2Impl_multiplyDispatcher;
	Calculator2Impl_vTable.copyMemory := @Calculator2Impl_copyMemoryDispatcher;
	Calculator2Impl_vTable.copyMemory2 := @Calculator2Impl_copyMemory2Dispatcher;

finalization
	DisposableImpl_vTable.destroy;
	StatusImpl_vTable.destroy;
	FactoryImpl_vTable.destroy;
	CalculatorImpl_vTable.destroy;
	Calculator2Impl_vTable.destroy;

end.
