cmake_minimum_required(VERSION 3.20)
project(SQL)

set(CMAKE_CXX_STANDARD 17)

add_executable(SQL main.cpp Database/B-Tree/Node/Node.h Database/B-Tree/BTNode/BTNode.cpp Database/B-Tree/BTNode/BTNode.h Database/B-Tree/BTree/BTree.cpp Database/B-Tree/BTree/BTree.h Database/Database.cpp Database/Database.h Database/Table/Table.cpp Database/Table/Table.h Database/Field/Field.cpp Database/Field/Field.h Database/Record/Record.cpp Database/Record/Record.h Database/Type/Type.h Database/Column/Column.cpp Database/Column/Column.h Database/Operator/Operator.h Database/Utils/Utils.cpp Database/Utils/Utils.h Database/Condition/Condition.cpp Database/Condition/Condition.h)