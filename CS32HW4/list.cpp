void listAll(const MenuItem* m, string path) // two-parameter overload
{
	
	if (m == NULL)
	{
		return;
	}
	else
	{
		if (path == "")
		{
			if (m->menuItems() != NULL)
				for (vector<MenuItem*>::const_iterator it = (*(m->menuItems())).cbegin(); it != (*(m->menuItems())).cend(); it++)
				{
					listAll(*it, (*it)->name());
				}
		}
		else
		{
			cout << path << endl;
			if (m->menuItems() != NULL)
				for (vector<MenuItem*>::const_iterator it = (*(m->menuItems())).cbegin(); it != (*(m->menuItems())).cend(); it++)
				{
					listAll(*it, path + "/" + (*it)->name());
				}
		}
	}
}