#include "pch.h"
#include "EmbPrjMcux.h"

EmbPrjMcux::EmbPrjMcux(string path_project_, string path_exe_)
{
	setPathExe(path_exe_);

	int a = path_project_.find_last_of('\\');
	int b = path_project_.find_last_of('/');
	int c = path_project_.find_last_of('.');
	a = _Max_value(a, b);
	string path = path_project_.substr(0, a + 1);//包含最后的那个'/'
	string name;
	if (path_project_.find(".project") != string::npos)
	{
	}
	else if (path_project_.find(".cproject") != string::npos)
	{
	}
	else
	{
		emb_assert(NULL, "格式错误");
	}
	path_project = path + ".project";
	path_cproject = path + ".cproject";


	XMLDocument ewp;
	XMLElement* titleElement;
	XMLElement* subElement;
	ewp.LoadFile(path_project.c_str());
	titleElement = ewp.FirstChildElement("projectDescription"); emb_assert(titleElement, "格式错误");
	titleElement = titleElement->FirstChildElement("name"); emb_assert(titleElement, "格式错误");
	name = titleElement->GetText();
	setPathProjectName(path, name);
	addPreprocessorDefinitions("__ARMVFP__");
	addPreprocessorDefinitions("__GNUC__");
}

EmbPrjMcux::~EmbPrjMcux(void)
{
}

int EmbPrjMcux::generater(void)
{
	searchDefinitions();
	searchIncludePaths();
	searchSourseItems();
	mcux11_1_0_LaterVersion();
	slnGenerate();
	vcxprojGenerate();
	filtersGenerate();
	userGenerate();
	return 0;
}

int EmbPrjMcux::cmakeGenner(void)
{
	//先判断存在不
	char contiuos = 'y';
	if (file_exists((getProjectDirectory() + "CMakeLists.txt").c_str()))
	{
		cout << getProjectDirectory() << endl;
		cout << "CMakeLists.txt exists" << " overwrite?(y/n):";
		contiuos = getchar();
	}
	if (contiuos != 'y')
	{
		return 0;
	}
	else//如果不存在就开始生成
	{
		searchCmake();
		ofstream cmake(getProjectDirectory() + "CMakeLists.txt");
		emb_assert(cmake.is_open(), "文件打不开");
		ofstream armgcc(getProjectDirectory() + "armgcc.cmake");
		emb_assert(armgcc.is_open(), "文件打不开");
		cmake << R"(MESSAGE("***Generaation Path=Debug or Release***"))" << endl;
		cmake << R"(MESSAGE("***Please set ARMGCC_DIR in envionment variables***"))" << endl;
		cmake << R"(MESSAGE("***example ARMGCC_DIR=C:\nxp\MCUXpressoIDE_11.0.1_2563\ide\tools ***"))" << endl;
		cmake << R"(MESSAGE("***cmake -DCMAKE_TOOLCHAIN_FILE="armgcc.cmake" -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=debug***") )" << endl;
		cmake << R"(MESSAGE("***cmake -DCMAKE_TOOLCHAIN_FILE="armgcc.cmake" -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=release***") )" << endl;

		//cmake << R"(MESSAGE( "***example ARMGCC_DIR=C:\Program Files (x86)\GNU Tools ARM Embedded\8 2019-q3-update ***"))" << endl;
		cmake <<
			R"(INCLUDE(CMakeForceCompiler)
# CROSS COMPILER SETTING
SET(CMAKE_SYSTEM_NAME Generic)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
# THE VERSION NUMBER
SET(Tutorial_VERSION_MAJOR 1)
SET(Tutorial_VERSION_MINOR 0)
# ENABLE ASM
ENABLE_LANGUAGE(ASM)
SET(CMAKE_STATIC_LIBRARY_PREFIX)
SET(CMAKE_STATIC_LIBRARY_SUFFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_PREFIX)
SET(CMAKE_EXECUTABLE_LIBRARY_SUFFIX)
# CURRENT DIRECTORY
SET(ProjDirPath ${CMAKE_CURRENT_SOURCE_DIR}))" << endl << endl;
		/*set< string> cmake_asm_flags_debug;
		set< string> cmake_asm_flags_release;
		set< string> cmake_c_flags_debug;
		set< string> cmake_c_flags_release;
		set< string> cmake_cxx_flags_debug;
		set< string> cmake_cxx_flags_release;
		set< string> cmake_exe_linker_flags_debug;
		set< string> cmake_exe_linker_flags_release;*/
		for (auto i : cmake_asm_flags_debug) { cmake << R"(SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} )" << i << R"("))" << endl; }
		for (auto i : cmake_asm_flags_release) { cmake << R"(SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} )" << i << R"("))" << endl; }
		for (auto i : cmake_c_flags_debug) { cmake << R"(SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} )" << i << R"("))" << endl; }
		for (auto i : cmake_c_flags_release) { cmake << R"(SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} )" << i << R"("))" << endl; }
		for (auto i : cmake_cxx_flags_debug) { cmake << R"(SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} )" << i << R"("))" << endl; }
		for (auto i : cmake_cxx_flags_release) { cmake << R"(SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} )" << i << R"("))" << endl; }
		for (auto i : cmake_exe_linker_flags_debug) { cmake << R"(SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} )" << i << R"("))" << endl; }
		for (auto i : cmake_exe_linker_flags_release) { cmake << R"(SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} )" << i << R"("))" << endl; }

		cmake << endl;
		cmake << R"(#include_directories(${ProjDirPath}/..))" << endl;
		for (auto i : getIncludePaths())
		{
			cmake << R"(include_directories(${ProjDirPath}/)" << i << ")" << endl;
		}

		cmake << "add_executable(" << getProjectName() << ".elf" << endl;
		for (auto i : getSourseItems())
		{
			if (i.find(".h") != string::npos
				|| i.find(".s") != string::npos
				|| i.find(".c") != string::npos
				)
			{
				string bbbbbb(i);
				replace_str(bbbbbb, "\\", "/");
				cmake << R"("${ProjDirPath}/)" << bbbbbb << "\"" << endl;
			}
		}
		cmake << ")" << endl;

		cmake << endl;
		cmake << R"(set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${ProjDirPath}/Debug/)" << getProjectName() << "_Debug" << R"(.ld -static"))" << endl;
		cmake << R"(set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T${ProjDirPath}/Release/)" << getProjectName() << "_Release" << R"(.ld -static"))" << endl;


		target_link_libraries.erase("-Wl,--end-group");
		target_link_libraries.erase("-Wl,--start-group");
		cmake << R"(TARGET_LINK_LIBRARIES()" << getProjectName() << ".elf " << "-Wl,--start-group" << ")" << endl;
		for (auto i : target_link_libraries)
		{
			cmake << R"(target_link_libraries()" << getProjectName() << ".elf " << i << ")" << endl;
		}
		cmake << R"(TARGET_LINK_LIBRARIES()" << getProjectName() << ".elf " << "-Wl,--end-group" << ")" << endl;




		cmake << "ADD_CUSTOM_COMMAND(" << endl
			<< "\tTARGET " << getProjectName() << ".elf" << endl
			<< "\tCOMMAND arm-none-eabi-size ${EXECUTABLE_OUTPUT_PATH}/" << getProjectName() << ".elf" << endl
			<< "\tCOMMAND arm-none-eabi-objcopy -v -O binary ${EXECUTABLE_OUTPUT_PATH}/" << getProjectName() << ".elf" << endl
			<< "\t${EXECUTABLE_OUTPUT_PATH}/" << getProjectName() << ".bin" << ")" << endl;


		cmake.close();

		armgcc << R"(INCLUDE(CMakeForceCompiler)

# TOOLCHAIN EXTENSION
IF(WIN32)
    SET(TOOLCHAIN_EXT ".exe")
ELSE()
    SET(TOOLCHAIN_EXT "")
ENDIF()

# EXECUTABLE EXTENSION
SET (CMAKE_EXECUTABLE_SUFFIX ".elf")

# TOOLCHAIN_DIR AND NANO LIBRARY
SET(TOOLCHAIN_DIR $ENV{ARMGCC_DIR})
STRING(REGEX REPLACE "\\\\" "/" TOOLCHAIN_DIR "${TOOLCHAIN_DIR}")

IF(NOT TOOLCHAIN_DIR)
    MESSAGE(FATAL_ERROR "***Please set ARMGCC_DIR in envionment variables***")
ENDIF()

MESSAGE(STATUS "TOOLCHAIN_DIR: " ${TOOLCHAIN_DIR})

# TARGET_TRIPLET
SET(TARGET_TRIPLET "arm-none-eabi")

SET(TOOLCHAIN_BIN_DIR ${TOOLCHAIN_DIR}/bin)
SET(TOOLCHAIN_INC_DIR ${TOOLCHAIN_DIR}/${TARGET_TRIPLET}/include)
SET(TOOLCHAIN_LIB_DIR ${TOOLCHAIN_DIR}/${TARGET_TRIPLET}/lib)

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR arm)

CMAKE_FORCE_C_COMPILER(${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-gcc${TOOLCHAIN_EXT} GNU)
#CMAKE_C_COMPILER(${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-gcc${TOOLCHAIN_EXT} GNU)
CMAKE_FORCE_CXX_COMPILER(${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-g++${TOOLCHAIN_EXT} GNU)
#CMAKE_CXX_COMPILER(${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-g++${TOOLCHAIN_EXT} GNU)
SET(CMAKE_ASM_COMPILER ${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-gcc${TOOLCHAIN_EXT})

SET(CMAKE_OBJCOPY ${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-objcopy CACHE INTERNAL "objcopy tool")
SET(CMAKE_OBJDUMP ${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-objdump CACHE INTERNAL "objdump tool")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g" CACHE INTERNAL "c compiler flags debug")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g" CACHE INTERNAL "cxx compiler flags debug")
SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -g" CACHE INTERNAL "asm compiler flags debug")
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}" CACHE INTERNAL "linker flags debug")

SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 " CACHE INTERNAL "c compiler flags release")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 " CACHE INTERNAL "cxx compiler flags release")
SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE}" CACHE INTERNAL "asm compiler flags release")
SET(CMAKE_EXE_LINKER_FLAGS_RELESE "${CMAKE_EXE_LINKER_FLAGS_RELESE}" CACHE INTERNAL "linker flags release")

SET(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_DIR}/${TARGET_TRIPLET} ${EXTRA_FIND_PATH})
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE})

MESSAGE(STATUS "BUILD_TYPE: " ${CMAKE_BUILD_TYPE})
)" << endl;
		armgcc.close();
		return 0;
	}
}

int EmbPrjMcux::file_exists(const char* filename)
{
	return (_access(filename, 0) == 0);
}

void EmbPrjMcux::mcux11_1_0_LaterVersion(void)
{
	//已经在searchDefinitions完成了对新版本的处理
}

void EmbPrjMcux::searchDefinitions(void)
{
	XMLDocument ewp;
	XMLElement* titleElement;
	XMLElement* subElement;
	string str;
	ewp.LoadFile(path_cproject.c_str());
	titleElement = ewp.FirstChildElement("cproject"); emb_assert(titleElement, "格式错误");
	titleElement = titleElement->FirstChildElement("storageModule"); emb_assert(titleElement, "格式错误");
	titleElement = titleElement->FirstChildElement("cconfiguration"); emb_assert(titleElement, "格式错误");
	str = titleElement->Attribute("id");
	//找debuG工程
	while (str.find("debug") == string::npos)
	{
		titleElement = titleElement->NextSiblingElement("cconfiguration");
		//emb_assert(titleElement, "格式错误");
		if (titleElement == nullptr)
		{
			titleElement = ewp.FirstChildElement("cproject")
				->FirstChildElement("storageModule")
				->FirstChildElement("cconfiguration");
			break;
		}
		str = titleElement->Attribute("id");
	}

	subElement = titleElement->FirstChildElement("storageModule"); emb_assert(subElement, "格式错误");
	str = subElement->Attribute("moduleId");
	//找build选项
	while (str.find("cdtBuildSystem") == string::npos)
	{
		subElement = subElement->NextSiblingElement("storageModule");
		//emb_assert(titleElement, "格式错误");
		if (titleElement == nullptr)
		{
			subElement = titleElement->FirstChildElement("storageModule"); emb_assert(subElement, "格式错误");
			break;
		}
		str = subElement->Attribute("moduleId");
	}

	titleElement = subElement->FirstChildElement("configuration");
	titleElement = titleElement->FirstChildElement("folderInfo");
	titleElement = titleElement->FirstChildElement("toolChain");
	subElement = titleElement->FirstChildElement("tool");
	str = subElement->Attribute("name");
	//找c语言编译器
	while (str.find("MCU C Compiler") == string::npos&&
		str.find("MCU GCC Compiler")==string::npos)
	{
		subElement = subElement->NextSiblingElement("tool");
		if (titleElement == nullptr)
		{
			subElement = titleElement->FirstChildElement("tool"); emb_assert(subElement, "格式错误");
			break;
		}
		str = subElement->Attribute("name");
	}

	titleElement = subElement;
	//开始查询预处理定义
	subElement = titleElement->FirstChildElement("option");
	str = subElement->Attribute("id");
	while (str.find("preprocessor.def.symbols") == string::npos &&
		str.find("compiler.option.definedsymbols")==string::npos)
	{
		subElement = subElement->NextSiblingElement("option");
		if (titleElement == nullptr)
		{
			subElement = titleElement->FirstChildElement("option"); emb_assert(subElement, "格式错误");
			break;
		}
		str = subElement->Attribute("id");
	}
	//添加预处理定义
	subElement = subElement->FirstChildElement("listOptionValue");
	while (subElement != nullptr)
	{
		addPreprocessorDefinitions(subElement->Attribute("value"));
		//cout << subElement->Attribute("value") << endl;
		subElement = subElement->NextSiblingElement("listOptionValue");
	}

	//开始查询inc目录
	subElement = titleElement->FirstChildElement("option");
	str = subElement->Attribute("id");
	while (str.find("compiler.option.include.paths") == string::npos &&
		str.find("compiler.option.includepaths")==string::npos)
	{
		subElement = subElement->NextSiblingElement("option");
		if (titleElement == nullptr)
		{
			subElement = titleElement->FirstChildElement("option"); emb_assert(subElement, "格式错误");
			break;
		}
		str = subElement->Attribute("id");
	}
	//添加inc目录
	subElement = subElement->FirstChildElement("listOptionValue");
	while (subElement != nullptr)
	{
		str = subElement->Attribute("value");

		if (str.find("$") != string::npos)
		{
			replace_str(str, "\"${workspace_loc:/${ProjName}/", "");
			replace_str(str, "}\"", "");
		}
		replace_str(str, "../", "");
		addIncludePaths(str);
		//cout << str << endl;
		subElement = subElement->NextSiblingElement("listOptionValue");
	}

	//cout << str << endl;

}

void EmbPrjMcux::searchIncludePaths(void)
{
	//由searchDefinitions完成了
}

void EmbPrjMcux::searchSourseItems(void)
{
	XMLDocument ewp;
	XMLElement* titleElement;
	XMLElement* subElement;
	string str;
	ewp.LoadFile(path_cproject.c_str());
	titleElement = ewp.FirstChildElement("cproject"); emb_assert(titleElement, "格式错误");
	titleElement = titleElement->FirstChildElement("storageModule"); emb_assert(titleElement, "格式错误");
	titleElement = titleElement->FirstChildElement("cconfiguration"); emb_assert(titleElement, "格式错误");
	str = titleElement->Attribute("id");
	//找debuG工程
	while (str.find("debug") == string::npos)
	{
		titleElement = titleElement->NextSiblingElement("cconfiguration");
		//emb_assert(titleElement, "格式错误");
		if (titleElement == nullptr)
		{
			titleElement = ewp.FirstChildElement("cproject")
				->FirstChildElement("storageModule")
				->FirstChildElement("cconfiguration");
			break;
		}
		str = titleElement->Attribute("id");
	}

	subElement = titleElement->FirstChildElement("storageModule"); emb_assert(subElement, "格式错误");
	str = subElement->Attribute("moduleId");
	//找build选项
	while (str.find("cdtBuildSystem") == string::npos)
	{
		subElement = subElement->NextSiblingElement("storageModule");
		//emb_assert(titleElement, "格式错误");
		if (titleElement == nullptr)
		{
			subElement = titleElement->FirstChildElement("storageModule"); emb_assert(subElement, "格式错误");
			break;
		}
		str = subElement->Attribute("moduleId");
	}
	//cout << str << endl;


	titleElement = subElement->FirstChildElement("configuration");
	titleElement = titleElement->FirstChildElement("sourceEntries");
	subElement = titleElement->FirstChildElement("entry");
	while (subElement != nullptr)
	{
		str = subElement->Attribute("name");
		//cout << str << endl;
		sourceEntries.insert(str);
		subElement = subElement->NextSiblingElement("entry");
	}
	for (auto j : sourceEntries)
	{
		vector<string> files;
		getFiles(getProjectDirectory() + j, files);
		//cout << endl << endl;
		//cout << getProjectDirectory() + j << endl;
		for (auto i : files)
		{
			//cout <<j/*.substr(0)*/+"\\"+ i.substr((getProjectDirectory() + j).size() + 1) << endl;
			addSourseItems(j + "\\" + i.substr((getProjectDirectory() + j).size() + 1));
		}
	}
	//cout << str << endl;

}

void EmbPrjMcux::searchCmake(void)
{
	//填充以下
	//set< string> cmake_asm_flags_debug;
	//set< string> cmake_asm_flags_release;
	//set< string> cmake_c_flags_debug;
	//set< string> cmake_c_flags_release;
	//set< string> cmake_cxx_flags_debug;
	//set< string> cmake_cxx_flags_release;
	//set< string> cmake_exe_linker_flags_debug;
	//set< string> cmake_exe_linker_flags_release;
	//set< string> target_link_libraries;

	//首先确定是哪个cpu,检查PreprocessorDefinitions
	string cpu;
	for (auto i : getPreprocessorDefinitions())
	{
		if (i.find("CPU") != string::npos)
		{
			cpu = i.substr(0, 9);
		}
	}
	cout << cpu << endl;
	XMLDocument ewp;
	XMLElement* titleElement;
	XMLElement* subElement;
	XMLElement* targetElement;
	string str;
	ewp.LoadFile((getPath_exe() + cmakexml).c_str());
	titleElement = ewp.FirstChildElement("Document"); emb_assert(titleElement, "格式错误");
	titleElement = titleElement->FirstChildElement("target"); emb_assert(titleElement, "格式错误");
	str = titleElement->Attribute("cpu");
	while (str.find(cpu) == string::npos)
	{
		titleElement = titleElement->NextSiblingElement("target");
		if (titleElement == nullptr)
		{
			titleElement = ewp.FirstChildElement("Document")->FirstChildElement("target");
			break;
		}
		str = titleElement->Attribute("cpu");
	}
	targetElement = titleElement;
	//已经确定好使哪个cpu了，开始处理

	//set< string> cmake_asm_flags_debug;
	subElement = targetElement->FirstChildElement("CMAKE_ASM_FLAGS_DEBUG"); emb_assert(subElement, "格式错误");
	subElement = subElement->FirstChildElement("item");
	while (subElement != nullptr)
	{
		cmake_asm_flags_debug.insert(subElement->GetText());
		subElement = subElement->NextSiblingElement("item");
	}

	//set< string> cmake_asm_flags_release;
	subElement = targetElement->FirstChildElement("CMAKE_ASM_FLAGS_RELEASE"); emb_assert(subElement, "格式错误");
	subElement = subElement->FirstChildElement("item");
	while (subElement != nullptr)
	{
		cmake_asm_flags_release.insert(subElement->GetText());
		subElement = subElement->NextSiblingElement("item");
	}

	//从元工程获取预定义
	searchDefDebugRelease();

	//set< string> cmake_c_flags_debug;
	subElement = targetElement->FirstChildElement("CMAKE_C_FLAGS_DEBUG"); emb_assert(subElement, "格式错误");
	subElement = subElement->FirstChildElement("def");
	while (subElement != nullptr)
	{
		DefDebug.insert(subElement->GetText());
		subElement = subElement->NextSiblingElement("def");
	}
	if (DefDebug.count("PRINTF_FLOAT_ENABLE=1")) { DefDebug.erase("PRINTF_FLOAT_ENABLE=0"); }
	if (DefDebug.count("PRINTF_ADVANCED_ENABLE=1")) { DefDebug.erase("PRINTF_ADVANCED_ENABLE=0"); }

	for (auto i : DefDebug)
	{
		cmake_c_flags_debug.insert("-D" + i);
	}
	subElement = targetElement->FirstChildElement("CMAKE_C_FLAGS_DEBUG"); emb_assert(subElement, "格式错误");
	subElement = subElement->FirstChildElement("item");
	while (subElement != nullptr)
	{
		cmake_c_flags_debug.insert(subElement->GetText());
		subElement = subElement->NextSiblingElement("item");
	}


	//set< string> cmake_c_flags_release;
	subElement = targetElement->FirstChildElement("CMAKE_C_FLAGS_RELEASE"); emb_assert(subElement, "格式错误");
	subElement = subElement->FirstChildElement("def");
	while (subElement != nullptr)
	{
		DefRelease.insert(subElement->GetText());
		subElement = subElement->NextSiblingElement("def");
	}
	if (DefRelease.count("PRINTF_FLOAT_ENABLE=1")) { DefRelease.erase("PRINTF_FLOAT_ENABLE=0"); }
	if (DefRelease.count("PRINTF_ADVANCED_ENABLE=1")) { DefRelease.erase("PRINTF_ADVANCED_ENABLE=0"); }
	for (auto i : DefRelease)
	{
		cmake_c_flags_release.insert("-D" + i);
	}
	subElement = targetElement->FirstChildElement("CMAKE_C_FLAGS_RELEASE"); emb_assert(subElement, "格式错误");
	subElement = subElement->FirstChildElement("item");
	while (subElement != nullptr)
	{
		cmake_c_flags_release.insert(subElement->GetText());
		subElement = subElement->NextSiblingElement("item");
	}
	//set< string> cmake_cxx_flags_debug;
	subElement = targetElement->FirstChildElement("CMAKE_CXX_FLAGS_DEBUG"); emb_assert(subElement, "格式错误");
	subElement = subElement->FirstChildElement("def");
	while (subElement != nullptr)
	{
		DefDebug.insert(subElement->GetText());
		subElement = subElement->NextSiblingElement("def");
	}
	for (auto i : DefDebug)
	{
		cmake_cxx_flags_debug.insert("-D" + i);
	}
	subElement = targetElement->FirstChildElement("CMAKE_CXX_FLAGS_DEBUG"); emb_assert(subElement, "格式错误");
	subElement = subElement->FirstChildElement("item");
	while (subElement != nullptr)
	{
		cmake_cxx_flags_debug.insert(subElement->GetText());
		subElement = subElement->NextSiblingElement("item");
	}
	//set< string> cmake_cxx_flags_release;
	subElement = targetElement->FirstChildElement("CMAKE_CXX_FLAGS_RELEASE"); emb_assert(subElement, "格式错误");
	subElement = subElement->FirstChildElement("def");
	while (subElement != nullptr)
	{
		DefRelease.insert(subElement->GetText());
		subElement = subElement->NextSiblingElement("def");
	}
	for (auto i : DefRelease)
	{
		cmake_cxx_flags_release.insert("-D" + i);
	}
	subElement = targetElement->FirstChildElement("CMAKE_CXX_FLAGS_RELEASE"); emb_assert(subElement, "格式错误");
	subElement = subElement->FirstChildElement("item");
	while (subElement != nullptr)
	{
		cmake_cxx_flags_release.insert(subElement->GetText());
		subElement = subElement->NextSiblingElement("item");
	}

	//set< string> cmake_exe_linker_flags_debug;
	subElement = targetElement->FirstChildElement("CMAKE_EXE_LINKER_FLAGS_DEBUG"); emb_assert(subElement, "格式错误");
	subElement = subElement->FirstChildElement("item");
	while (subElement != nullptr)
	{
		cmake_exe_linker_flags_debug.push_back(subElement->GetText());
		subElement = subElement->NextSiblingElement("item");
	}

	//set< string> cmake_exe_linker_flags_release;
	subElement = targetElement->FirstChildElement("CMAKE_EXE_LINKER_FLAGS_RELEASE"); emb_assert(subElement, "格式错误");
	subElement = subElement->FirstChildElement("item");
	while (subElement != nullptr)
	{
		cmake_exe_linker_flags_release.push_back(subElement->GetText());
		subElement = subElement->NextSiblingElement("item");
	}

	//set< string> target_link_libraries;
	subElement = targetElement->FirstChildElement("TARGET_LINK_LIBRARIES"); emb_assert(subElement, "格式错误");
	subElement = subElement->FirstChildElement("item");
	while (subElement != nullptr)
	{
		target_link_libraries.insert(subElement->GetText());
		subElement = subElement->NextSiblingElement("item");
	}


}

void EmbPrjMcux::searchDefDebugRelease(void)
{
	for (auto i : getPreprocessorDefinitions())
	{
		DefDebug.insert(i);
		if (i.find("debug") == string::npos
			&& i.find("Debug") == string::npos
			&& i.find("DEBUG") == string::npos
			)
		{
			DefRelease.insert(i);
		}
	}

	DefDebug.erase("__GNUC__");
	DefRelease.erase("__GNUC__");
}
