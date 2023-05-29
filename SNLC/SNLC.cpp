// SNLC.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "SNLC.h"

// 设计条件编译标志，满足不同编译功能需求
int main()
{
    getTokenList("./Docs/snl3.txt", "./Docs/token.txt");
    TreeNode* root = DesParse();
    Analyze(root);
    outputTreeNode(root);
    cout << "语法树输出完成" << endl;
    CodeFile* c = GenMidCode(root);

    std::cout << "Hello World!\n";

    system("PAUSE");
}

void outputTreeNode(TreeNode* root) {
    if (root == nullptr) {
        return;
    }

     //输出当前节点的属性
    std::cout << "Line: " << root->lineno << std::endl;
    std::cout << "Node Kind: " << root->nodekind << std::endl;
    
    //std::cout << "Kind: " << root->kind << std::endl;
    std::cout << "Id Num: " << root->idnum << std::endl;

    std::cout << "Names: ";
    for (const auto& name : root->name) {
        std::cout << name << " ";
    }
    std::cout << std::endl;

    std::cout << "Tables: ";
    for (const auto& table : root->table) {
        std::cout << table << " ";
    }
    std::cout << std::endl;

    std::cout << "Type Name: " << root->type_name << std::endl;

     //输出其他属性，根据需要进行适当的输出
    cout << "----------------------" << endl;

     //递归遍历子节点
    for (int i = 0; i < 3; ++i) {
        outputTreeNode(root->child[i]);
    }

     //遍历兄弟节点
    outputTreeNode(root->sibling);
}


//void outputTreeNode(TreeNode* tree)
//{
//	int space = 0, error = 0 ;
//	space = space + 4;
//	//FILE* show = fopen("../Grammer.txt", "w+");
//
//	while (tree != NULL)
//	{
//		for (int i = 0; i < space; i++)
//		{
//			////fprintf(show, " ");
//			printf(" ");
//		}
//		switch (tree->nodekind)
//		{
//		case ProK:
//		{
//			////fprintf(show, "%s  ", "ProK");
//			printf("%s  ", "ProK");
//			break;
//		}
//		case PheadK:
//		{
//			for (int i = 0; i < 5; i++) {
//				////fprintf(show, " ");
//				printf(" ");
//			}
//			////fprintf(show, "%s  ", "PheadK");
//			//fprintf(show, "%s  ", tree->name[0]);
//			printf("%s  ", "PheadK");
//			printf("%s  ", tree->name[0]);
//		}
//		break;
//		case DecK:
//		{
//
//			//fprintf(show, "%s  ", "DecK");
//			printf("%s  ", "DecK");
//			switch (tree->kind.dec)
//			{
//			case  ArrayK:
//			{
//
//				//fprintf(show, "%s  ", "ArrayK"); printf("%s  ", "ArrayK");
//				//fprintf(show, "%d  ", tree->attr.ArrayAttr.low); printf("%d  ", tree->attr.ArrayAttr.low);
//				//fprintf(show, "%d  ", tree->attr.ArrayAttr.up); printf("%d  ", tree->attr.ArrayAttr.up);
//				if (tree->attr.arrayAttr.childType == CharK) {
//					//fprintf(show, "%s  ", "Chark"); printf("%s  ", "Chark");
//				}
//				else if (tree->attr.arrayAttr.childType == IntegerK) {
//					//fprintf(show, "%s  ", "IntegerK"); printf("%s  ", "IntegerK");
//				}
//			}; break;
//			case  CharK:
//				//fprintf(show, "%s  ", "CharK");
//				printf("%s  ", "CharK");
//				break;
//			case  IntegerK:
//				//fprintf(show, "%s  ", "IntegerK");
//				printf("%s  ", "IntegerK");
//				break;
//			case  RecordK:
//				//fprintf(show, "%s  ", "RecordK");
//				printf("%s  ", "RecordK");
//				break;
//			case  IdK:
//				//fprintf(show, "%s  ", "IdK");
//				//fprintf(show, "%s  ", tree->type_name);
//				printf("%s  ", "IdK");
//				printf("%s  ", tree->type_name);
//				break;
//			default:
//				//fprintf(show, "error1!");
//				printf("error1!");
//				error = 1;
//			};
//			if (tree->idnum != 0)
//				for (int i = 0; i <= (tree->idnum); i++)
//				{
//					//fprintf(show, "%s  ", tree->name[i]);
//					printf("%s  ", tree->name[i]);
//				}
//			else
//			{
//				//fprintf(show, "wrong!no var!\n");
//				printf("wrong!no var!\n");
//				error = 1;
//			}
//		} break;
//		case TypeK:
//		{
//			//fprintf(show, "%s  ", "TypeK");
//			printf("%s  ", "TypeK");
//			break;
//		}
//		case VarK:
//
//			//fprintf(show, "%s  ", "VarK");
//			printf("%s  ", "VarK");
//			break;
//
//		case ProcDecK:
//
//			//fprintf(show, "%s  ", "ProcDecK");
//			//fprintf(show, "%s  ", tree->name[0]);
//			printf("%s  ", "ProcDecK");
//			printf("%s  ", tree->name[0]);
//			break;
//
//		case StmLK:
//			//fprintf(show, "%s  ", "StmLk");
//			printf("%s  ", "StmLk");
//			break;
//
//		case StmtK:
//		{
//			//fprintf(show, "%s  ", "StmtK");
//			printf("%s  ", "StmtK");
//			switch (tree->kind.stmt)
//			{
//			case IfK:
//				//fprintf(show, "%s  ", "If");
//				printf("%s  ", "If");
//				break;
//			case WhileK:
//				//fprintf(show, "%s  ", "While");
//				printf("%s  ", "While");
//				break;
//
//			case AssignK:
//				//fprintf(show, "%s  ", "Assign");
//				printf("%s  ", "Assign");
//				break;
//
//			case ReadK:
//				//fprintf(show, "%s  ", "Read");
//				//fprintf(show, "%s  ", tree->name[0]);
//				printf("%s  ", "Read");
//				printf("%s  ", tree->name[0]);
//				break;
//
//			case WriteK:
//				//fprintf(show, "%s  ", "Write");
//				printf("%s  ", "Write");
//				break;
//
//			case CallK:
//				//fprintf(show, "%s  ", "Call");
//				//fprintf(show, "%s  ", tree->name[0]);
//				printf("%s  ", "Call");
//				printf("%s  ", tree->name[0]);
//				break;
//
//			case ReturnK:
//				//fprintf(show, "%s  ", "Return");
//				printf("%s  ", "Return");
//				break;
//
//			default:
//				//fprintf(show, "error2!");
//				printf("error2!");
//				error = 1;
//			}
//		}; break;
//		case ExpK:
//		{
//			//fprintf(show, "%s  ", "ExpK"); printf("%s  ", "ExpK");
//			switch (tree->kind.exp)
//			{
//			case OpK:
//			{ //fprintf(show, "%s  ", "Op"); printf("%s  ", "Op");
//			switch (tree->attr.expAttr.op)
//			{
//			case EQ:   //fprintf(show, "%s  ", "="); printf("%s  ", "="); break;
//			case LT:   //fprintf(show, "%s  ", "<"); printf("%s  ", "<"); break;
//			case PLUS: //fprintf(show, "%s  ", "+"); printf("%s  ", "+"); break;
//			case MINUS://fprintf(show, "%s  ", "-"); printf("%s  ", "-"); break;
//			case TIMES://fprintf(show, "%s  ", "*"); printf("%s  ", "*"); break;
//			case OVER: //fprintf(show, "%s  ", "/"); printf("%s  ", "/"); break;
//			default:
//				//fprintf(show, "error3!");
//				printf("error3!");
//				error = 1;
//			}
//
//			if (tree->attr.expAttr.varkind == ArrayMembV)
//			{
//				//fprintf(show, "ArrayMember  ");
//				//fprintf(show, "%s  ", tree->name[0]);
//				printf("ArrayMember  ");
//				printf("%s  ", tree->name[0]);
//			}
//			}; break;
//			case ConstK:
//				//fprintf(show, "%s  ", "Const");
//				printf("%s  ", "Const");
//				switch (tree->attr.expAttr.varkind)
//				{
//				case IdV:
//					//fprintf(show, "%s  ", tree->name[0]);
//					printf("%s  ", tree->name[0]);
//					break;
//				case FieldMembV:
//					//fprintf(show, "%s  ", tree->name[0]);
//					printf("%s  ", tree->name[0]);
//					break;
//				case ArrayMembV:
//					//fprintf(show, "%s  ", tree->name[0]);
//					printf("%s  ", tree->name[0]);
//					break;
//				default:
//					//fprintf(show, "ConstK type error!");
//					printf("ConstK type error!");
//					error = 1;
//				}
//				//fprintf(show, "%d  ", tree->attr.ExpAttr.val);
//				printf("%d  ", tree->attr.expAttr.val);
//				break;
//			//case VariK:
//			//	switch (tree->attr.expAttr.varkind)
//			//	{
//			//	case IdV:
//			//		//fprintf(show, "%s  ", tree->name[0]); printf("%s  ", tree->name[0]);
//			//		//fprintf(show, "IdV  "); printf("IdV  ");
//			//		break;
//			//	case FieldMembV:
//			//		//fprintf(show, "%s  ", tree->name[0]); printf("%s  ", tree->name[0]);
//			//		//fprintf(show, "FieldMembV  "); printf("FieldMembV  ");
//			//		break;
//			//	case ArrayMembV:
//			//		//fprintf(show, "%s  ", tree->name[0]); printf("%s  ", tree->name[0]);
//			//		//fprintf(show, "ArrayMembV  "); printf("ArrayMembV  ");
//			//		break;
//			//	default:
//			//		//fprintf(show, "var type error!");
//			//		error = 1;
//			//		printf("var type error!");
//			//	}
//			//	break;
//			default:
//				//fprintf(show, "error4!");
//				printf("error4!");
//				error = 1;
//			}
//		}; break;
//		default:
//			//fprintf(show, "error5!");
//			printf("error5!");
//			error = 1;
//		}
//
//		//fprintf(show, "\n");
//		printf("\n");
//		for (int i = 0; i < 2; i++)
//		{
//			outputTreeNode(tree->child[i]);
//		}
//
//		tree = tree->sibling;
//	}
//	space = space - 4;
//}
//

/// vs
/// 批量注释 Ctrl+K+C
/// 取消注释 Ctrl+K+U
///