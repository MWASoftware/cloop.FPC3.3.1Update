# Updated cloop for generating the Pascal Interface for FPC 3.3.x

The reason for the proposed update is that future versions of the Free Pascal Compiler (3.3.1 onwards) change the object instance layout and hence break the current Firebird OO API interface. The proposed change is intended to rectify this with minimal change to user applications.
The change adds a new field to the object instance layout immediately after the vmt pointer and before any user fields. It thus conflicts with the cloop assumption that the layout of an object instance is a vmt pointer following by the user fields. The object's vTable field is thus not where it is expected to be and any use of the Firebird OOAPI results in a crash.

The purpose of this new field is for supporting a monitoring interface. It is understood that a similar function is being added to Delphi but using a “hidden field”. It is not clear if the same problem applies to Delphi. However, the proposed update is intended to work with both FPC and Delphi without making any assumptions about the object instance layout.

The proposed update has been tested with the Firebird Pascal API (fbintf) for both use of the services provided by the Firebird engThe reason for the proposed update is that future versions of the Free Pascal Compiler (3.3.1 onwards) change the object instance layout and hence break the current Firebird OO API interface. The proposed change is intended to rectify this with minimal change to user applications. The generated Pascal unit is now packaged with fbintf.

The change adds a new field to the object instance layout immediately after the vmt pointer and before any user fields. It thus conflicts with the cloop assumption that the layout of an object instance is a vmt pointer following by the user fields. The object's vTable field is thus not where it is expected to be and any use of the Firebird OOAPI results in a crash.

The purpose of this new field is for supporting a monitoring interface. It is understood that a similar function is being added to Delphi but using a “hidden field”. It is not clear if the same problem applies to Delphi. However, the proposed update is intended to work with both FPC and Delphi without making any assumptions about the object instance layout.

The proposed Pascal generator is a full rewrite of the previous version and, in summary:

    1. The vTable structure is changed from a class hierarchy to a record declaration for each interface. The change to a record is to avoid any dependencies on the object instance layout. However, record definitions do not support inheritance and hence each vTable declaration has to be “complete” i.e. it comprises: a null pointer, followed by a version number and then a pointer to each method starting with the top level interface. 

The result is arguably an improvement given that, unlike classes, record variables can be initialised at compile time. In the proposed update, all implementation vTables are compile time initialised resulting the removal of the initialization and finalization sections - hopefully less bloat and faster loading.

    2. The “interface classes” are replaced by a pointer to a record and the record declaration. Again this is to avoid any dependencies on the object instance layout e.g. IMaster is now a pointer to TMaster, and the TMaster record comprises: a null pointer and a vTable pointer. The Pascal extended record declaration syntax is used and hence each record also includes method declarations for each of the interface's methods. As with vTables, the full list of methods has to be declared starting with the top level interface. The implementation of each method follows the previous approach of calling the corresponding vTable method.

    3. The “implementation classes” are still classes with abstract methods but form their own inheritance hierarchy independent of the “interfaces” (e.g. IMasterImpl becomes a subclass of IVersionedImpl and not IMaster). These classes are abstract classes and hence could not be implemented as records. The class declaration defines two fields: a null pointer followed by a pointer to the implementation vTable as assigned by the class constructor.

    4. Implicit type coercions between implementations (e.g. IMasterImpl instances) and interfaces (IMaster) is no longer possible and an explicit coercion is necessary. Each “implementation” class declaration includes such a coercion. For example, the class declaration for IMasterImpl includes
function.asIMaster: IMaster

The implementation of this function simply returns a pointer to the object's null pointer field coerced to the appropriate interface pointer e.g.

function IMasterImpl.asIMaster:IMaster;
begin
  Result := IMaster(@FNullPtr);
end;

    5. The despatcher methods for each implementation vTable are largely as before, except that a means is necessary to locate the corresponding object instance so that the appropriate method can be called. The despatcher method is called with an interface pointer i.e. a pointer to the implementation class's null pointer field - and not the object itself. In order to avoid any assumptions about the object instance layout, in the implementation class the vTable pointer field is always followed by an object reference initialised to “self” by the class constructor. It is this field that is used to locate the object instance.

    6. The above is implemented as two additional methods for each interface (extended) record. For example TMaster includes:

	  function isIMasterImpl: boolean;
	  function asIMasterImpl: IMasterImpl;

These are intended to replace the functionality that would previously have been provided by the Pascal “is” and “as” operators when testing the relationship between interface and implementation classes.

The “is” method compares the interface's vTable pointer against the implementation class's Implementation vTable (and each of its subclasses) and returns true if it finds a match.

The “as” method also calls the “is” method and if this returns true, the method assumes that it is safe to access the object reference following the vTable pointer and returns this reference coerced to the expected object instance. Otherwise, the method raises an exception.
The “as” method is used in the despatcher functions for each implementation vTable. The “is” and “as” methods may also be useful in UDRs whenever there is a need to access an underlying implementation object.

    7. Interface constant definitions cannot be included in the record declarations (not supported by Pascal) and instead are defined in the implementation classes. In order to avoid breaking existing code, inline methods with the same name as each constant are included in the extended records. The implementation of these methods simply returns the constant value as defined in the implementation class. The constants are directly available to each implementation class.ine and with the UDR support library. It is now shipping with the most recent version of IBX. 

  
