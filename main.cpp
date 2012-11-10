#include "stdafx.h"
#include "SchemeRuntime.h"

int main(int argc, char **argv)
{
	if(argc > 1)
	{
		SchemeRuntime runtime;
		for(int i=1;i<argc;i++)
		{
			ifstream fin(argv[i], ios::in);
			if(fin)
			{
				string text;
				string *tmp = new string();
				while(getline(fin, *tmp))
				{
					text.append(*tmp + "\n");
				}
				fin.close();
				delete tmp;
				runtime.loadStr(text);
				try {
					runtime.lexAnalise();
					runtime.run();
				} catch (Exception* e){
					cout << e->what() << endl;;
					delete e;
				}
			}
			else
			{
				cout << "File '" << argv[i] << " doesn't exist" << endl;
			}
		}
	}
	else
	{
		cout << "please pass in scheme file paths as files to be excuted" << endl;
	}

	return 0;
}
