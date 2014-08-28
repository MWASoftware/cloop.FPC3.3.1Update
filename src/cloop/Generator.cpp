#include "Generator.h"
#include "Expr.h"
#include <deque>
#include <set>
#include <string>
#include <vector>

using std::deque;
using std::set;
using std::string;
using std::vector;


//--------------------------------------


const char* const Generator::AUTOGEN_MSG =
	"This file was autogenerated by cloop - Cross Language Object Oriented Programming";


//--------------------------------------


FileGenerator::FileGenerator(const string& filename)
{
	out = fopen(filename.c_str(), "w+");
}

FileGenerator::~FileGenerator()
{
	fclose(out);
}


//--------------------------------------


CBasedGenerator::CBasedGenerator(const string& filename, bool cPlusPlus)
	: FileGenerator(filename),
	  cPlusPlus(cPlusPlus)
{
}

string CBasedGenerator::convertType(const Type& type)
{
	string ret(type.isConst ? "const " : "");

	switch (type.token.type)
	{
		case Token::TYPE_IDENTIFIER:
			ret += string(cPlusPlus ? "" : "struct ") + type.token.text + "*";
			break;

		default:
			ret += type.token.text;
			break;
	}

	if (type.isPointer)
		ret += "*";

	return ret;
}


//--------------------------------------


CppGenerator::CppGenerator(const string& filename, Parser* parser, const string& headerGuard,
		const string& className)
	: CBasedGenerator(filename, true),
	  parser(parser),
	  headerGuard(headerGuard),
	  className(className)
{
}

void CppGenerator::generate()
{
	fprintf(out, "// %s\n\n", AUTOGEN_MSG);

	fprintf(out, "#ifndef %s\n", headerGuard.c_str());
	fprintf(out, "#define %s\n\n", headerGuard.c_str());
	fprintf(out, "#include <stdint.h>\n\n\n");

	fprintf(out, "template <typename Policy>\n");
	fprintf(out, "class %s\n", className.c_str());
	fprintf(out, "{\n");
	fprintf(out, "public:\n");

	fprintf(out, "\t// Interfaces declarations\n\n");

	for (vector<Interface*>::iterator i = parser->interfaces.begin();
		 i != parser->interfaces.end();
		 ++i)
	{
		Interface* interface = *i;

		deque<Method*> methods;

		for (Interface* p = interface; p; p = p->super)
			methods.insert(methods.begin(), p->methods.begin(), p->methods.end());

		if (!interface->super)
			fprintf(out, "\tclass %s\n", interface->name.c_str());
		else
		{
			fprintf(out, "\tclass %s : public %s\n",
				interface->name.c_str(), interface->super->name.c_str());
		}

		fprintf(out, "\t{\n");
		fprintf(out, "\tprotected:\n");

		if (!interface->super)
		{
			fprintf(out, "\t\tstruct VTable\n");
			fprintf(out, "\t\t{\n");
			fprintf(out, "\t\t\tvoid* cloopDummy[%d];\n", DUMMY_VTABLE);
			fprintf(out, "\t\t\tuintptr_t version;\n");
		}
		else
		{
			fprintf(out, "\t\tstruct VTable : public %s::VTable\n",
				interface->super->name.c_str());
			fprintf(out, "\t\t{\n");
		}

		for (vector<Method*>::iterator j = interface->methods.begin();
			 j != interface->methods.end();
			 ++j)
		{
			Method* method = *j;

			fprintf(out, "\t\t\t%s (*%s)(%s%s* self",
				convertType(method->returnType).c_str(),
				method->name.c_str(),
				(method->isConst ? "const " : ""),
				interface->name.c_str());

			for (vector<Parameter*>::iterator k = method->parameters.begin();
				 k != method->parameters.end();
				 ++k)
			{
				Parameter* parameter = *k;

				fprintf(out, ", %s %s",
					convertType(parameter->type).c_str(), parameter->name.c_str());
			}

			fprintf(out, ");\n");
		}

		fprintf(out, "\t\t};\n");
		fprintf(out, "\n");

		if (!interface->super)
		{
			fprintf(out, "\tprotected:\n");
			fprintf(out, "\t\tvoid* cloopDummy[%d];\n", DUMMY_INSTANCE);
			fprintf(out, "\t\tVTable* cloopVTable;\n");
			fprintf(out, "\n");
		}

		fprintf(out, "\tpublic:\n");
		fprintf(out, "\t\tstatic const int VERSION = %d;\n", (int) methods.size());

		if (!interface->constants.empty())
			fprintf(out, "\n");

		for (vector<Constant*>::iterator j = interface->constants.begin();
			 j != interface->constants.end();
			 ++j)
		{
			Constant* constant = *j;

			fprintf(out, "\t\tstatic const %s %s = %s;\n",
				convertType(constant->type).c_str(),
				constant->name.c_str(),
				constant->expr->generate(LANGUAGE_CPP).c_str());
		}

		unsigned methodNumber = (interface->super ? interface->super->methods.size() : 0);
		for (vector<Method*>::iterator j = interface->methods.begin();
			 j != interface->methods.end();
			 ++j)
		{
			Method* method = *j;

			fprintf(out, "\n");
			fprintf(out, "\t\t%s %s(",
				convertType(method->returnType).c_str(), method->name.c_str());

			for (vector<Parameter*>::iterator k = method->parameters.begin();
				 k != method->parameters.end();
				 ++k)
			{
				Parameter* parameter = *k;

				if (k != method->parameters.begin())
					fprintf(out, ", ");

				fprintf(out, "%s %s",
					convertType(parameter->type).c_str(), parameter->name.c_str());
			}

			fprintf(out, ")%s\n", (method->isConst ? " const" : ""));
			fprintf(out, "\t\t{\n");
			fprintf(out, "\t\t\tPolicy::template checkVersion<%d>(this);\n", ++methodNumber);

			fprintf(out, "\t\t\t");

			if (method->returnType.token.type != Token::TYPE_VOID)
			{
				fprintf(out, "%s ret = ", convertType(method->returnType).c_str());
				//// TODO: Policy::upgrade
			}

			fprintf(out, "static_cast<VTable*>(this->cloopVTable)->%s(this",
				method->name.c_str());

			for (vector<Parameter*>::iterator k = method->parameters.begin();
				 k != method->parameters.end();
				 ++k)
			{
				Parameter* parameter = *k;
				fprintf(out, ", %s", parameter->name.c_str());
			}

			fprintf(out, ");\n");

			if (!method->parameters.empty() &&
				parser->exceptionInterface &&
				method->parameters.front()->type.token.text == parser->exceptionInterface->name)
			{
				fprintf(out, "\t\t\tPolicy::checkException(%s);\n",
					method->parameters.front()->name.c_str());
			}

			if (method->returnType.token.type != Token::TYPE_VOID)
				fprintf(out, "\t\t\treturn ret;\n");

			fprintf(out, "\t\t}\n");
		}

		fprintf(out, "\t};\n\n");
	}

	fprintf(out, "\t// Interfaces implementations\n");

	for (vector<Interface*>::iterator i = parser->interfaces.begin();
		 i != parser->interfaces.end();
		 ++i)
	{
		Interface* interface = *i;

		deque<Method*> methods;

		for (Interface* p = interface; p; p = p->super)
			methods.insert(methods.begin(), p->methods.begin(), p->methods.end());

		fprintf(out, "\n");
		fprintf(out, "\ttemplate <typename Name, typename Base>\n");
		fprintf(out, "\tclass %sBaseImpl : public Base\n", interface->name.c_str());
		fprintf(out, "\t{\n");
		fprintf(out, "\tpublic:\n");
		fprintf(out, "\t\t%sBaseImpl()\n", interface->name.c_str());
		fprintf(out, "\t\t{\n");
		fprintf(out, "\t\t\tstatic struct VTableImpl : Base::VTable\n");
		fprintf(out, "\t\t\t{\n");
		fprintf(out, "\t\t\t\tVTableImpl()\n");
		fprintf(out, "\t\t\t\t{\n");
		fprintf(out, "\t\t\t\t\tthis->version = Base::VERSION;\n");

		for (deque<Method*>::iterator j = methods.begin(); j != methods.end(); ++j)
		{
			Method* method = *j;

			fprintf(out, "\t\t\t\t\tthis->%s = &Name::cloop%sDispatcher;\n",
				method->name.c_str(), method->name.c_str());
		}

		fprintf(out, "\t\t\t\t}\n");
		fprintf(out, "\t\t\t} vTable;\n");
		fprintf(out, "\n");

		fprintf(out, "\t\t\tthis->cloopVTable = &vTable;\n");
		fprintf(out, "\t\t}\n");

		// We generate all bases dispatchers so indirect overrides work. At the same time, we
		// inherit from all bases impls, so pure virtual methods are introduced and required to
		// be overriden in the user's implementation.

		for (Interface* p = interface; p; p = p->super)
		{
			for (vector<Method*>::iterator j = p->methods.begin(); j != p->methods.end(); ++j)
			{
				Method* method = *j;

				fprintf(out, "\n");
				fprintf(out, "\t\tstatic %s cloop%sDispatcher(%s%s* self",
					convertType(method->returnType).c_str(),
					method->name.c_str(),
					(method->isConst ? "const " : ""),
					p->name.c_str());

				for (vector<Parameter*>::iterator k = method->parameters.begin();
					 k != method->parameters.end();
					 ++k)
				{
					Parameter* parameter = *k;

					fprintf(out, ", %s %s",
						convertType(parameter->type).c_str(), parameter->name.c_str());
				}

				fprintf(out, ") throw()\n");
				fprintf(out, "\t\t{\n");
				fprintf(out, "\t\t\ttry\n");
				fprintf(out, "\t\t\t{\n");

				fprintf(out, "\t\t\t\t");

				if (method->returnType.token.type != Token::TYPE_VOID)
					fprintf(out, "return ");

				fprintf(out, "static_cast<%sName*>(self)->Name::%s(",
					(method->isConst ? "const " : ""),
					method->name.c_str());

				for (vector<Parameter*>::iterator k = method->parameters.begin();
					 k != method->parameters.end();
					 ++k)
				{
					Parameter* parameter = *k;

					if (k != method->parameters.begin())
						fprintf(out, ", ");

					fprintf(out, "%s", parameter->name.c_str());
				}

				fprintf(out, ");\n");

				Parameter* exceptionParameter =
					(!method->parameters.empty() &&
					 parser->exceptionInterface &&
					 method->parameters.front()->type.token.text == parser->exceptionInterface->name
					) ? method->parameters.front() : NULL;

				fprintf(out, "\t\t\t}\n");
				fprintf(out, "\t\t\tcatch (...)\n");
				fprintf(out, "\t\t\t{\n");
				fprintf(out, "\t\t\t\tPolicy::catchException(%s);\n",
					(exceptionParameter ? exceptionParameter->name.c_str() : "0"));

				if (method->returnType.token.type != Token::TYPE_VOID)
				{
					fprintf(out, "\t\t\t\treturn static_cast<%s>(0);\n",
						convertType(method->returnType).c_str());
				}

				fprintf(out, "\t\t\t}\n");

				fprintf(out, "\t\t}\n");
			}
		}

		fprintf(out, "\t};\n\n");

		if (!interface->super)
		{
			fprintf(out, "\ttemplate <typename Name, typename Base = %s>\n",
				interface->name.c_str());
		}
		else
		{
			string base;
			unsigned baseCount = 0;

			for (Interface* p = interface->super; p; p = p->super)
			{
				base += p->name + "Impl<Name, ";
				++baseCount;
			}

			base += interface->name;

			while (baseCount-- > 0)
				base += "> ";

			fprintf(out, "\ttemplate <typename Name, typename Base = %s>\n", base.c_str());
		}

		fprintf(out, "\tclass %sImpl : public %sBaseImpl<Name, Base>\n",
			interface->name.c_str(), interface->name.c_str());
		fprintf(out, "\t{\n");
		fprintf(out, "\tpublic:\n");
		fprintf(out, "\t\tvirtual ~%sImpl()\n", interface->name.c_str());
		fprintf(out, "\t\t{\n");
		fprintf(out, "\t\t}\n");
		fprintf(out, "\n");

		for (vector<Method*>::iterator j = interface->methods.begin();
			 j != interface->methods.end();
			 ++j)
		{
			Method* method = *j;

			fprintf(out, "\t\tvirtual %s %s(",
				convertType(method->returnType).c_str(), method->name.c_str());

			for (vector<Parameter*>::iterator k = method->parameters.begin();
				 k != method->parameters.end();
				 ++k)
			{
				Parameter* parameter = *k;

				if (k != method->parameters.begin())
					fprintf(out, ", ");

				fprintf(out, "%s %s",
					convertType(parameter->type).c_str(), parameter->name.c_str());
			}

			fprintf(out, ")%s = 0;\n", (method->isConst ? " const" : ""));
		}

		fprintf(out, "\t};\n");
	}

	fprintf(out, "};\n\n\n");

	fprintf(out, "#endif\t// %s\n", headerGuard.c_str());
}


//--------------------------------------


CHeaderGenerator::CHeaderGenerator(const string& filename, Parser* parser,
		const string& headerGuard)
	: CBasedGenerator(filename, false),
	  parser(parser),
	  headerGuard(headerGuard)
{
}

void CHeaderGenerator::generate()
{
	fprintf(out, "/* %s */\n\n", AUTOGEN_MSG);

	fprintf(out, "#ifndef %s\n", headerGuard.c_str());
	fprintf(out, "#define %s\n\n", headerGuard.c_str());
	fprintf(out, "#include <stdint.h>\n\n");

	fprintf(out, "#ifndef CLOOP_EXTERN_C\n");
	fprintf(out, "#ifdef __cplusplus\n");
	fprintf(out, "#define CLOOP_EXTERN_C extern \"C\"\n");
	fprintf(out, "#else\n");
	fprintf(out, "#define CLOOP_EXTERN_C\n");
	fprintf(out, "#endif\n");
	fprintf(out, "#endif\n\n\n");

	for (vector<Interface*>::iterator i = parser->interfaces.begin();
		 i != parser->interfaces.end();
		 ++i)
	{
		Interface* interface = *i;

		deque<Method*> methods;

		for (Interface* p = interface; p; p = p->super)
			methods.insert(methods.begin(), p->methods.begin(), p->methods.end());

		fprintf(out, "#define %s_VERSION %d\n\n",
			interface->name.c_str(), (int) methods.size());

		for (vector<Constant*>::iterator j = interface->constants.begin();
			 j != interface->constants.end();
			 ++j)
		{
			Constant* constant = *j;

			fprintf(out, "#define %s_%s ((%s) (%s))\n",
				interface->name.c_str(),
				constant->name.c_str(),
				convertType(constant->type).c_str(),
				constant->expr->generate(LANGUAGE_C).c_str());
		}

		if (!interface->constants.empty())
			fprintf(out, "\n");

		fprintf(out, "struct %s;\n\n", interface->name.c_str());

		fprintf(out, "struct %sVTable\n", interface->name.c_str());
		fprintf(out, "{\n");
		fprintf(out, "\tvoid* cloopDummy[%d];\n", DUMMY_VTABLE);
		fprintf(out, "\tuintptr_t version;\n");

		for (deque<Method*>::iterator j = methods.begin(); j != methods.end(); ++j)
		{
			Method* method = *j;

			fprintf(out, "\t%s (*%s)(%sstruct %s* self",
				convertType(method->returnType).c_str(),
				method->name.c_str(),
				(method->isConst ? "const " : ""),
				interface->name.c_str());

			for (vector<Parameter*>::iterator k = method->parameters.begin();
				 k != method->parameters.end();
				 ++k)
			{
				Parameter* parameter = *k;

				fprintf(out, ", %s %s", convertType(parameter->type).c_str(),
					parameter->name.c_str());
			}

			fprintf(out, ");\n");
		}

		fprintf(out, "};\n\n");

		fprintf(out, "struct %s\n", interface->name.c_str());
		fprintf(out, "{\n");
		fprintf(out, "\tvoid* cloopDummy[%d];\n", DUMMY_INSTANCE);
		fprintf(out, "\tstruct %sVTable* vtable;\n", interface->name.c_str());
		fprintf(out, "};\n\n");

		for (deque<Method*>::iterator j = methods.begin(); j != methods.end(); ++j)
		{
			Method* method = *j;

			fprintf(out, "CLOOP_EXTERN_C %s %s_%s(%sstruct %s* self",
				convertType(method->returnType).c_str(),
				interface->name.c_str(),
				method->name.c_str(),
				(method->isConst ? "const " : ""),
				interface->name.c_str());

			for (vector<Parameter*>::iterator k = method->parameters.begin();
				 k != method->parameters.end();
				 ++k)
			{
				Parameter* parameter = *k;

				fprintf(out, ", %s %s",
					convertType(parameter->type).c_str(), parameter->name.c_str());
			}

			fprintf(out, ");\n");
		}

		fprintf(out, "\n");
	}

	fprintf(out, "\n");
	fprintf(out, "#endif\t// %s\n", headerGuard.c_str());
}


//--------------------------------------


CImplGenerator::CImplGenerator(const string& filename, Parser* parser,
		const string& includeFilename)
	: CBasedGenerator(filename, false),
	  parser(parser),
	  includeFilename(includeFilename)
{
}

void CImplGenerator::generate()
{
	fprintf(out, "/* %s */\n\n", AUTOGEN_MSG);

	fprintf(out, "#include \"%s\"\n\n\n", includeFilename.c_str());

	for (vector<Interface*>::iterator i = parser->interfaces.begin();
		 i != parser->interfaces.end();
		 ++i)
	{
		Interface* interface = *i;

		deque<Method*> methods;

		for (Interface* p = interface; p; p = p->super)
			methods.insert(methods.begin(), p->methods.begin(), p->methods.end());

		for (deque<Method*>::iterator j = methods.begin(); j != methods.end(); ++j)
		{
			Method* method = *j;

			fprintf(out, "CLOOP_EXTERN_C %s %s_%s(%sstruct %s* self",
				convertType(method->returnType).c_str(),
				interface->name.c_str(),
				method->name.c_str(),
				(method->isConst ? "const " : ""),
				interface->name.c_str());

			for (vector<Parameter*>::iterator k = method->parameters.begin();
				 k != method->parameters.end();
				 ++k)
			{
				Parameter* parameter = *k;

				fprintf(out, ", %s %s",
					convertType(parameter->type).c_str(), parameter->name.c_str());
			}

			fprintf(out, ")\n");
			fprintf(out, "{\n");
			fprintf(out, "\t");

			if (method->returnType.token.type != Token::TYPE_VOID)
				fprintf(out, "return ");

			fprintf(out, "self->vtable->%s(self", method->name.c_str());

			for (vector<Parameter*>::iterator k = method->parameters.begin();
				 k != method->parameters.end();
				 ++k)
			{
				Parameter* parameter = *k;
				fprintf(out, ", %s", parameter->name.c_str());
			}

			fprintf(out, ");\n");
			fprintf(out, "}\n\n");
		}
	}
}


//--------------------------------------


//// TODO: Generate constants (including VERSION).
PascalGenerator::PascalGenerator(const string& filename, Parser* parser, const string& unitName)
	: FileGenerator(filename),
	  parser(parser),
	  unitName(unitName)
{
}

void PascalGenerator::generate()
{
	fprintf(out, "{ %s }\n\n", AUTOGEN_MSG);

	fprintf(out, "unit %s;\n\n", unitName.c_str());
	fprintf(out, "interface\n\n");
	fprintf(out, "uses Classes;\n\n");
	fprintf(out, "type\n");

	for (vector<Interface*>::iterator i = parser->interfaces.begin();
		 i != parser->interfaces.end();
		 ++i)
	{
		Interface* interface = *i;
		fprintf(out, "\t%s = class;\n", interface->name.c_str());
	}

	fprintf(out, "\n");

	// Pass at every type to fill pointerTypes. We need it in advance.

	for (vector<Interface*>::iterator i = parser->interfaces.begin();
		 i != parser->interfaces.end();
		 ++i)
	{
		Interface* interface = *i;

		for (vector<Method*>::iterator j = interface->methods.begin();
			 j != interface->methods.end();
			 ++j)
		{
			Method* method = *j;

			convertType(method->returnType);

			for (vector<Parameter*>::iterator k = method->parameters.begin();
				 k != method->parameters.end();
				 ++k)
			{
				Parameter* parameter = *k;
				convertParameter(*parameter);
			}
		}
	}

	for (set<string>::iterator i = pointerTypes.begin(); i != pointerTypes.end(); ++i)
	{
		string type = *i;
		fprintf(out, "\t%sPtr = ^%s;\n", type.c_str(), type.c_str());
	}

	if (!pointerTypes.empty())
		fprintf(out, "\n");

	for (vector<Interface*>::iterator i = parser->interfaces.begin();
		 i != parser->interfaces.end();
		 ++i)
	{
		Interface* interface = *i;

		for (vector<Method*>::iterator j = interface->methods.begin();
			 j != interface->methods.end();
			 ++j)
		{
			Method* method = *j;

			fprintf(out, "\t%s_%sPtr = %s(this: %s",
				interface->name.c_str(), method->name.c_str(),
				(method->returnType.token.type == Token::TYPE_VOID ? "procedure" : "function"),
				interface->name.c_str());

			for (vector<Parameter*>::iterator k = method->parameters.begin();
				 k != method->parameters.end();
				 ++k)
			{
				Parameter* parameter = *k;
				fprintf(out, "; %s", convertParameter(*parameter).c_str());
			}

			fprintf(out, ")");

			if (method->returnType.token.type != Token::TYPE_VOID)
				fprintf(out, ": %s", convertType(method->returnType).c_str());

			fprintf(out, "; cdecl;\n");
		}
	}

	fprintf(out, "\n");

	for (vector<Interface*>::iterator i = parser->interfaces.begin();
		 i != parser->interfaces.end();
		 ++i)
	{
		Interface* interface = *i;

		fprintf(out, "\t%sVTable = class", interface->name.c_str());

		if (interface->super)
			fprintf(out, "(%sVTable)", interface->super->name.c_str());

		fprintf(out, "\n");

		if (!interface->super)
		{
			fprintf(out, "{$ifndef FPC}\n");
			fprintf(out, "\t\tdummy: PtrInt;\n");
			fprintf(out, "{$endif}\n");
			fprintf(out, "\t\tversion: PtrInt;\n");
		}

		for (vector<Method*>::iterator j = interface->methods.begin();
			 j != interface->methods.end();
			 ++j)
		{
			Method* method = *j;

			fprintf(out, "\t\t%s: %s_%sPtr;\n", method->name.c_str(), interface->name.c_str(),
				method->name.c_str());
		}

		fprintf(out, "\tend;\n\n");

		fprintf(out, "\t%s = class", interface->name.c_str());

		if (interface->super)
			fprintf(out, "(%s)", interface->super->name.c_str());

		fprintf(out, "\n");

		if (!interface->super)
		{
			fprintf(out, "{$ifndef FPC}\n");
			fprintf(out, "\t\tdummy: PtrInt;\n");
			fprintf(out, "{$endif}\n");
			fprintf(out, "\t\tvTable: %sVTable;\n", interface->name.c_str());
		}

		for (vector<Method*>::iterator j = interface->methods.begin();
			 j != interface->methods.end();
			 ++j)
		{
			Method* method = *j;

			fprintf(out, "\t\t%s %s(",
				(method->returnType.token.type == Token::TYPE_VOID ? "procedure" : "function"),
				method->name.c_str());

			for (vector<Parameter*>::iterator k = method->parameters.begin();
				 k != method->parameters.end();
				 ++k)
			{
				Parameter* parameter = *k;

				if (k != method->parameters.begin())
					fprintf(out, "; ");

				fprintf(out, "%s", convertParameter(*parameter).c_str());
			}

			fprintf(out, ")");

			if (method->returnType.token.type != Token::TYPE_VOID)
				fprintf(out, ": %s", convertType(method->returnType).c_str());

			fprintf(out, ";\n");
		}

		fprintf(out, "\tend;\n\n");

		fprintf(out, "\t%sImpl = class(%s)\n",
			interface->name.c_str(), interface->name.c_str());
		fprintf(out, "\t\tconstructor create;\n\n");

		deque<Method*> methods;

		for (Interface* p = interface; p; p = p->super)
			methods.insert(methods.begin(), p->methods.begin(), p->methods.end());

		for (deque<Method*>::iterator j = methods.begin(); j != methods.end(); ++j)
		{
			Method* method = *j;

			fprintf(out, "\t\t%s %s(",
				(method->returnType.token.type == Token::TYPE_VOID ? "procedure" : "function"),
				method->name.c_str());

			for (vector<Parameter*>::iterator k = method->parameters.begin();
				 k != method->parameters.end();
				 ++k)
			{
				Parameter* parameter = *k;

				if (k != method->parameters.begin())
					fprintf(out, "; ");

				fprintf(out, "%s", convertParameter(*parameter).c_str());
			}

			fprintf(out, ")");

			if (method->returnType.token.type != Token::TYPE_VOID)
				fprintf(out, ": %s", convertType(method->returnType).c_str());

			fprintf(out, "; virtual; abstract;\n");
		}

		fprintf(out, "\tend;\n\n");
	}

	fprintf(out, "implementation\n\n");

	for (vector<Interface*>::iterator i = parser->interfaces.begin();
		 i != parser->interfaces.end();
		 ++i)
	{
		Interface* interface = *i;

		for (vector<Method*>::iterator j = interface->methods.begin();
			 j != interface->methods.end();
			 ++j)
		{
			Method* method = *j;

			fprintf(out, "%s %s.%s(",
				(method->returnType.token.type == Token::TYPE_VOID ? "procedure" : "function"),
				interface->name.c_str(),
				method->name.c_str());

			for (vector<Parameter*>::iterator k = method->parameters.begin();
				 k != method->parameters.end();
				 ++k)
			{
				Parameter* parameter = *k;

				if (k != method->parameters.begin())
					fprintf(out, "; ");

				fprintf(out, "%s", convertParameter(*parameter).c_str());
			}

			fprintf(out, ")");

			if (method->returnType.token.type != Token::TYPE_VOID)
				fprintf(out, ": %s", convertType(method->returnType).c_str());

			fprintf(out, ";\n");
			fprintf(out, "begin\n");
			fprintf(out, "\t");

			if (method->returnType.token.type != Token::TYPE_VOID)
				fprintf(out, "Result := ");

			fprintf(out, "%sVTable(vTable).%s(Self",
				interface->name.c_str(), method->name.c_str());

			for (vector<Parameter*>::iterator k = method->parameters.begin();
				 k != method->parameters.end();
				 ++k)
			{
				Parameter* parameter = *k;
				fprintf(out, ", %s", parameter->name.c_str());
			}

			fprintf(out, ");\n");
			fprintf(out, "end;\n\n");
		}
	}

	for (vector<Interface*>::iterator i = parser->interfaces.begin();
		 i != parser->interfaces.end();
		 ++i)
	{
		Interface* interface = *i;

		deque<Method*> methods;

		for (Interface* p = interface; p; p = p->super)
			methods.insert(methods.begin(), p->methods.begin(), p->methods.end());

		for (deque<Method*>::iterator j = methods.begin(); j != methods.end(); ++j)
		{
			Method* method = *j;

			fprintf(out, "%s %sImpl_%sDispatcher(this: %s",
				(method->returnType.token.type == Token::TYPE_VOID ? "procedure" : "function"),
				interface->name.c_str(),
				method->name.c_str(),
				interface->name.c_str());

			for (vector<Parameter*>::iterator k = method->parameters.begin();
				 k != method->parameters.end();
				 ++k)
			{
				Parameter* parameter = *k;

				fprintf(out, "; %s", convertParameter(*parameter).c_str());
			}

			fprintf(out, ")");

			if (method->returnType.token.type != Token::TYPE_VOID)
				fprintf(out, ": %s", convertType(method->returnType).c_str());

			fprintf(out, "; cdecl;\n");
			fprintf(out, "begin\n");
			fprintf(out, "\t");

			if (method->returnType.token.type != Token::TYPE_VOID)
				fprintf(out, "Result := ");

			fprintf(out, "%sImpl(this).%s(", interface->name.c_str(), method->name.c_str());

			for (vector<Parameter*>::iterator k = method->parameters.begin();
				 k != method->parameters.end();
				 ++k)
			{
				Parameter* parameter = *k;

				if (k != method->parameters.begin())
					fprintf(out, ", ");

				fprintf(out, "%s", parameter->name.c_str());
			}

			fprintf(out, ");\n");
			fprintf(out, "end;\n\n");
		}

		fprintf(out, "var\n");
		fprintf(out, "\t%sImpl_vTable: %sVTable;\n\n",
			interface->name.c_str(), interface->name.c_str());

		fprintf(out, "constructor %sImpl.create;\n", interface->name.c_str());
		fprintf(out, "begin\n");
		fprintf(out, "\tvTable := %sImpl_vTable;\n", interface->name.c_str());
		fprintf(out, "end;\n\n");
	}

	fprintf(out, "initialization\n");

	for (vector<Interface*>::iterator i = parser->interfaces.begin();
		 i != parser->interfaces.end();
		 ++i)
	{
		Interface* interface = *i;

		deque<Method*> methods;

		for (Interface* p = interface; p; p = p->super)
			methods.insert(methods.begin(), p->methods.begin(), p->methods.end());

		fprintf(out, "\t%sImpl_vTable := %sVTable.create;\n",
			interface->name.c_str(), interface->name.c_str());
		fprintf(out, "\t%sImpl_vTable.version := %d;\n",
			interface->name.c_str(), (int) methods.size());

		for (deque<Method*>::iterator j = methods.begin(); j != methods.end(); ++j)
		{
			Method* method = *j;

			fprintf(out, "\t%sImpl_vTable.%s := @%sImpl_%sDispatcher;\n",
				interface->name.c_str(),
				method->name.c_str(),
				interface->name.c_str(),
				method->name.c_str());
		}

		fprintf(out, "\n");
	}

	fprintf(out, "finalization\n");

	for (vector<Interface*>::iterator i = parser->interfaces.begin();
		 i != parser->interfaces.end();
		 ++i)
	{
		Interface* interface = *i;
		fprintf(out, "\t%sImpl_vTable.destroy;\n", interface->name.c_str());
	}

	fprintf(out, "\n");
	fprintf(out, "end.\n");
}

string PascalGenerator::convertParameter(const Parameter& parameter)
{
	return parameter.name + ": " + convertType(parameter.type);
}

string PascalGenerator::convertType(const Type& type)
{
	string name;

	switch (type.token.type)
	{
		case Token::TYPE_INT:
			name = "Integer";
			break;

		default:
			name = type.token.text;
			break;
	}

	if (type.isPointer)
	{
		if (pointerTypes.find(name) == pointerTypes.end())
			pointerTypes.insert(name);

		name += "Ptr";
	}

	return name;
}
