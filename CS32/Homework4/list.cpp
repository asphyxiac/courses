// @file: list.ccp
void listAllAuxiliary(string path, const MenuItem* m)
{
    
    if (!m)
        return;

    if ( m->menuItems() )
    {
        const vector<MenuItem*> *menu = (m->menuItems());
        
        for (vector<MenuItem*>::const_iterator it = (*menu).begin(); it < (*menu).end(); ++it)
        {
            cout << path+(*it)->name() << endl;
            listAllAuxiliary( path+(*it)->name()+'/', *it);
        }
    } 
}