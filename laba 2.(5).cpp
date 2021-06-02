// laba 2.(5).cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//


#include <iostream>
#include <stdio.h>
//#include <windows.h>
//#include <direct.h> 
using namespace std;

struct vocabulary
{
	char ukr[20];
	char eng[20];
	int count;
};
struct Node
{
	vocabulary data;
	Node* left;
	Node* right;
};

//проверки

int input1_2(char(&a)[100])
// функция проверяет ввод пользователя
{
	if (a[0] == 49)
		return 1;
	else if (a[0] == 50)
		return 2;
	else
	{
		return 0;
	}
}

int input1_2_3_4(char(&a)[100])
// функция проверяет ввод пользователя
{
	if (a[0] == 49)
		return 1;
	else if (a[0] == 50)
		return 2;
	else if (a[0] == 51)
		return 3;
	else if (a[0] == 52)
		return 4;
	else
	{
		return 0;
	}
}

bool check_input(char* a, int size)
// функция проверяет ввод пользователя
{
	for (int i = 0; i < size; i++)
		if ((a[i] > 32 && a[i] < 65) || (a[i] > 90 && a[i] < 97) || (a[i] > 122))
		{
			return 0;
		}
	return 1;
}

int ch(char* mas1, char* mas2, int n)
// функция проверяет два массива на схожесть и возращает ближайший номер ячеек, которые отличаются
{
	int count = 0;
	for (int i = 0; i < n; i++)
	{
		if (mas1[i] != mas2[i])
			return count;
		count++;
	}

}

bool check(char* mas1, char* mas2, int n)
// функция возврщает TRUE, если массивы одинаковые, а противном случае - FALSE
{
	for (int i = 0; i < n; i++)
		if (mas1[i] != mas2[i])
			return 0;
	return 1;
}

//создание дерева

void AddToTree_Alphabetically(Node*& Tree_Alphabetically, vocabulary data)
// функция для добавления элементов в дерево, которое строится по алфавиту 
{
	int i = 0;
	if (Tree_Alphabetically && data.eng[0] == Tree_Alphabetically->data.eng[0])
		i = ch(data.eng, Tree_Alphabetically->data.eng, size(data.eng));
	if (!Tree_Alphabetically)
	{
		Tree_Alphabetically = new Node;
		Tree_Alphabetically->data = data;
		Tree_Alphabetically->left = NULL;
		Tree_Alphabetically->right = NULL;
	}
	else if (data.eng[i] < Tree_Alphabetically->data.eng[i])
		AddToTree_Alphabetically(Tree_Alphabetically->left, data);
	else
		AddToTree_Alphabetically(Tree_Alphabetically->right, data);
}

void CreatTree_Alphabetically(Node*& Tree_Alphabetically)
// функция для постройки дерева словаря по алфавиту (считывает с файла слова и добавляет их в дерево)
{
	FILE* dict;
	vocabulary word;
	if (fopen_s(&dict, "dictionary", "rb"))
	{
		perror("Error");
	}
	else
	{
		while (!feof(dict))
		{
			fread(&word, sizeof(vocabulary), 1, dict);
			if (feof(dict))
				break;
			AddToTree_Alphabetically(Tree_Alphabetically, word);
		}
	}
	fclose(dict);
}

//построение дерева по показчику обращений

void AddToTree_ВyСounter(Node*& Tree_ByCounter, vocabulary data)
// функция для добавления элементов в дерево, котрое строится по количеству обращений к слову
{
	if (!Tree_ByCounter)
	{
		Tree_ByCounter = new Node;
		Tree_ByCounter->data = data;
		Tree_ByCounter->left = NULL;
		Tree_ByCounter->right = NULL;
	}
	else if (data.count <= Tree_ByCounter->data.count)
		AddToTree_ВyСounter(Tree_ByCounter->left, data);
	else
		AddToTree_ВyСounter(Tree_ByCounter->right, data);
}

void TreeSearch_max(Node* Tree_Alphabetically, int& max, vocabulary& word)
// функция ищет элемент с максимальным значением обращений 
{
	if (Tree_Alphabetically)
	{
		TreeSearch_max(Tree_Alphabetically->left, max, word);
		if (Tree_Alphabetically->data.count >= max)
		{
			max = Tree_Alphabetically->data.count;
			word = Tree_Alphabetically->data;
		}
		TreeSearch_max(Tree_Alphabetically->right, max, word);
	}
}

void DeleteMass(char(&mas)[20])
//функция удаляет значения масива, обращая их в NULL
{
	for (int i = 0; i < 20; i++)
		mas[i] = NULL;
}

void TreeDeleteData(Node*& Tree_Alphabetically, vocabulary word)
// функция "удаляет" элемент дерева с максимальным значением обращений к слову
{
	if (Tree_Alphabetically)
	{
		TreeDeleteData(Tree_Alphabetically->left, word);
		if (check(Tree_Alphabetically->data.eng, word.eng, size(word.eng)))
		{
			DeleteMass(Tree_Alphabetically->data.eng);
			DeleteMass(Tree_Alphabetically->data.ukr);
			Tree_Alphabetically->data.count = -1;
		}
		TreeDeleteData(Tree_Alphabetically->right, word);
	}
}

void boo(Node* Tree, bool& b)
// функция проверяет не исчерпалось ли дерево
{
	if (Tree)
	{
		boo(Tree->left, b);
		if (Tree->data.count != -1)
			b = true;
		else boo(Tree->right, b);
	}
}

void FreeTree(Node*& Tree)
// функция удаляет дерево (освобождает память)
{
	if (!Tree) return;
	FreeTree(Tree->left);
	FreeTree(Tree->right);
	delete Tree;
	return;
}

void CreatTree_ByCounter(Node*& Tree_Alphabetically, Node*& Tree_ByCounter)
// функция для постройки дерева по количеству обращений к функции
{
	int max = 0;
	bool b = 0;
	vocabulary word;
	TreeSearch_max(Tree_Alphabetically, max, word); // ищет слово с максимальным значением обращений
	TreeDeleteData(Tree_Alphabetically, word);// удляет это слово со старого дерева (которое было построено по алфавиту)
	AddToTree_ВyСounter(Tree_ByCounter, word);// добавляет слово с найбольшим значеним в дерево
	boo(Tree_Alphabetically, b);// проверяет старое дерево на пустоту
	if (b)
		CreatTree_ByCounter(Tree_Alphabetically, Tree_ByCounter);// если старое дерево не пустое, то рекурсивно вызываем эту же функцию
	FreeTree(Tree_Alphabetically);// когда старое дерево стало пустим - удаляем его (освобождаем память)
	Tree_Alphabetically = NULL;// оставляем лишь корень, который делаем пустым
}

//изменение показчика

void count_plus_one(vocabulary word1)
// функция добавляет 1 после каждого обращения к слову (word.count++ записывается в файл и изменения вносяться в дерево)
{
	FILE* dictionary1;
	FILE* dictionary2;
	if (fopen_s(&dictionary1, "dictionary", "rb") || fopen_s(&dictionary2, "dictionary_copy", "wb"))
		perror("Error");
	else
	{
		vocabulary word2;
		while (!feof(dictionary1))
		{
			fread(&word2, sizeof(vocabulary), 1, dictionary1);
			if (feof(dictionary1))
				break;
			fwrite(&word2, sizeof(vocabulary), 1, dictionary2);
		}
		fclose(dictionary1);
		fclose(dictionary2);

	}
	if (fopen_s(&dictionary1, "dictionary", "wb") || fopen_s(&dictionary2, "dictionary_copy", "rb"))
		perror("Error");
	else
	{
		vocabulary word2;
		while (!feof(dictionary2))
		{
			fread(&word2, sizeof(vocabulary), 1, dictionary2);
			if (feof(dictionary2))
				break;
			if (check(word1.eng, word2.eng, size(word2.eng)))
				word2.count++;
			fwrite(&word2, sizeof(vocabulary), 1, dictionary1);
		}
		fclose(dictionary1);
		fclose(dictionary2);
	}
	remove("dictionary_copy");
}

void TreeSearch_translation(Node*& Tree_Alphabetically, vocabulary& word)
// по английскому варианту слова функция ищет украинский вариант в дереве
{
	if (Tree_Alphabetically)
	{
		TreeSearch_translation(Tree_Alphabetically->left, word);
		if (check(Tree_Alphabetically->data.eng, word.eng, size(word.eng)))
		{
			word = Tree_Alphabetically->data;
			Tree_Alphabetically->data.count++;
		}
		TreeSearch_translation(Tree_Alphabetically->right, word);
	}
}

void translate(Node* Tree_Alphabetically)
//функция для перевода слов на украинский
{
	vocabulary word;
	cout << "Enter word to translate: ";
	cin.getline(word.eng, 20, '\n');
	if (!check_input(word.eng, size(word.eng)))
		return;

	TreeSearch_translation(Tree_Alphabetically, word);
	cout << word.eng<<"-" << word.ukr << endl << endl;
	count_plus_one(word);
}

//второстепенные функции

void print_tree(Node* Tree, int level)
//функция печатает дерево
{
	if (Tree)
	{
		print_tree(Tree->left, level + 1);
		for (int i = 0; i < level; i++)
			cout << "     ";
		cout << Tree->data.eng << "-" << Tree->data.count << endl;
		print_tree(Tree->right, level + 1);
	}
}

void add_to_dictionary()
//функция добавляет в файл новые слова
{
	vocabulary word;
	cout << "Enter the English word: ";
	cin.getline(word.eng, 20, '\n');
	if (!check_input(word.eng, size(word.eng)))
		return;

	cout << "Enter the translation: ";
	cin.getline(word.ukr, 20, '\n');
	if (!check_input(word.ukr, size(word.eng)))
		return;

	cout << "Enter the counter: ";
	cin >> word.count;

	FILE* dict;
	fopen_s(&dict, "dictionary", "a");
	fclose(dict);
	if (fopen_s(&dict, "dictionary", "rb"))
		perror("Error");
	else
	{
		vocabulary word2;
		while (!feof(dict))
		{
			fread(&word2, sizeof(vocabulary), 1, dict);
			if (feof(dict))
				break;
			if (check(word.eng, word2.eng, size(word.eng)))
			{
				cout << endl<<"This word already exists in the dictionary" << endl;
				fclose(dict);
				return;
			}
		}
	}
	fclose(dict);

	if (fopen_s(&dict, "dictionary", "a+b"))
		perror("Error");
	else
		fwrite(&word, sizeof(word), 1, dict);
	fclose(dict);
}

bool pause()
// функция заканчивает работу программы, если пользователь больше не хочет продолжать работу
{
	while (1)
	{
		cout << endl;
		cout << "Continue?" << endl
			<< "1 yes" << endl << "2 no" << endl << " " << endl;
		char s[100];
		cin >> s;
		cout << endl;
		int k;
		k = input1_2(s);
		switch (k)
		{
		case 1:
			return 1;
			break;
		case 2:
			return 0;
			break;
		default:
			break;
		}
	}
}



int main()
{
	Node* Tree_Alphabetically = NULL;
	Node* Tree_ByCounter = NULL;
	setlocale(LC_ALL, " ");
	int k, cou1 = 0, cou2 = 0;
	bool b = 1;
	while (b)
	{
		cout<< "1) Add a new word. " << endl
			<< "2) Build a dictionary. " << endl
			<< "3) Translation. " << endl
			<< "4) Build a counter tree. " << endl
			<< " " << endl;
		char s[100];
		cin.getline(s, 100, '\n');
		cout << endl;
		k = input1_2_3_4(s);
		if (k != 0)
		{
			switch (k)
			{
			case 1:
				add_to_dictionary();
				b = pause();
				break;
			case 2:
				if (cou1 != 0)
				{
					FreeTree(Tree_Alphabetically);
					Tree_Alphabetically = NULL;
				}
				CreatTree_Alphabetically(Tree_Alphabetically);
				print_tree(Tree_Alphabetically, 0);
				cout << endl;
				b = pause();
				cou1++;
				break;
			case 3:
				if (cou1 == 0)
					CreatTree_Alphabetically(Tree_Alphabetically);
				cou1++;
				translate(Tree_Alphabetically);
				b = pause();
				break;
		
			case 4:
				if (cou2 != 0)
				{
					FreeTree(Tree_ByCounter);
					Tree_ByCounter = NULL;
				}
				if (!Tree_Alphabetically || Tree_Alphabetically->data.count == -1)
					CreatTree_Alphabetically(Tree_Alphabetically);
				CreatTree_ByCounter(Tree_Alphabetically, Tree_ByCounter);
				print_tree(Tree_ByCounter, 0);
				cout << endl;
				b = pause();
				cou2++;
			default:
				break;
			}
			cin.ignore(32767, '\n');
		}
	}
}
