#include <iostream>
#include <fstream>
#include <cassert>
using namespace std;

class Item
{
public:
    string name;
    string url;
#if 0
    int r, g, b;
#endif

private:
    Item* link;
    Item* prev;
    Item* next;
    Item* first;
    Item* last;
    void insert(Item* before, Item* fresh, Item* after);

public:
    Item* getLink() { return link; }

    Item* getPrev() { return prev; }
    Item* getNext() { return next; }

    Item* getFirst() { return first; }
    Item* getLast() { return last; }

    void insertFirst(Item* p);
    void insertLast(Item* p);

    void insertPrev(Item* fresh);
    void insertNext(Item* fresh);

    void remove();

    Item* search(string name) const;
    void print(int level = 0) const;
    int length() const;

    void indent(ostream& f, int level) const;
    void send(ostream& f, int level = 0) const;
    void html(ostream& f) const;

#if 0
    Item(string name0, int r0, int g0, int b0);
#endif
    Item(string name0 = "", string url0 = "");
    ~Item();

    static int count;
};

int Item::count = 0;

#if 0
Item::Item(string name0, int r0, int g0, int b0)
{
    name = name0;
    url = "";
    r = r0;
    g = g0;
    b = b0;
    link = nullptr;
    prev = nullptr;
    next = nullptr;
    first = nullptr;
    last = nullptr;
    count++;
}
#endif

Item::Item(string name0, string url0)
{
    name = name0;
    url = url0;
#if 0
    r = 0;
    g = 0;
    b = 0;
#endif
    link = nullptr;
    prev = nullptr;
    next = nullptr;
    first = nullptr;
    last = nullptr;
    count++;
}

Item::~Item()
{
    count--;
    if (link != nullptr)
        remove();

    Item* p = first;
    while (p != nullptr)
    {
        Item* t = p->next; // ukazatel na nasledujici prvek

        p->link = nullptr;
        p->prev = nullptr;
        p->next = nullptr;
        delete p;

        p = t;
    }
    first = nullptr;
    last = nullptr;
}

void Item::insert(Item* before, Item* fresh, Item* after)
{
    assert(fresh != nullptr);
    assert(fresh->link == nullptr);
    assert(fresh->prev == nullptr);
    assert(fresh->next == nullptr);

    fresh->link = this;
    fresh->prev = before;
    fresh->next = after;

    if (before == nullptr)
        first = fresh;
    else
        before->next = fresh;

    if (after == nullptr)
        last = fresh;
    else
        after->prev = fresh;
}

void Item::remove()
{
    if (link != nullptr)
    {
        Item* before = prev;
        Item* after = next;

        if (before != nullptr)
            before->next = after;
        else
            link->first = after;

        if (after != nullptr)
            after->prev = before;
        else
            link->last = before;

        link = nullptr;
        prev = nullptr;
        next = nullptr;
    }
}

void Item::insertFirst(Item* p)
{
    insert(nullptr, p, first);
}

void Item::insertLast(Item* p)
{
    this->insert(this->last, p, nullptr);
}

void Item::insertPrev(Item* fresh)
{
    assert(this != nullptr);
    assert(link != nullptr);
    link->insert(prev, fresh, this);
}

void Item::insertNext(Item* fresh)
{
    assert(this != nullptr);
    assert(link != nullptr);
    this->link->insert(this, fresh, this->next);
}

Item* Item::search(string name) const
{
    Item* p = first;
    while (p != nullptr && p->name != name)
    {
        p = p->next;
    }
    return p;
}

void Item::print(int level) const
{
    for (int i = 1; i <= level; i++)
        cout << "    ";

    cout << name << " " << url << endl;

    Item* p = first;
    while (p != nullptr)
    {
        p->print(level + 1);
        // cout << p->name << " (" << p->r << ", " << p->g << ", " << p->b << ")" << endl;
        p = p->next;
    }
}

void Item::indent(ostream& f, int level) const
{
    for (int i = 1; i <= level; i++)
        f << "    ";
}
void Item::send(ostream& f, int level) const
{
    indent(f, level);
    f << "<li>";

    if (url != "")
    {
        f << "<a href=\"" + url + "\"> " + name + "</a>";
        f << endl;
        indent(f, level);
        f << R"...(<img src=")..." + url + R"...(" </img>)...";
    }
    else
    {
        f << name;
    }

    f << "</li>" << endl;

    if (first != nullptr)
    {
        indent(f, level + 1);
        f << "<ul>" << endl;

        Item* p = first;
        while (p != nullptr)
        {
            p->send(f, level + 2);
            p = p->next;
        }

        indent(f, level + 1);
        f << "</ul>" << endl;
    }
}

void Item::html(ostream& f) const
{
    f << "<!DOCTYPE html>" << endl;
    f << "<html>" << endl;
    f << "<head>" << endl;
    f << "</head>" << endl;
    f << "<body>" << endl;

    f << "<ul>" << endl;
    send(f);
    f << "</ul>" << endl;

    f << "</body>" << endl;
    f << "</html>" << endl;
}

int Item::length() const
{
    int cnt = 0;

    Item* p = first;
    while (p != nullptr)
    {
        cnt++;
        p = p->next;
    }

    return cnt;
}

#if 0
void test()
{
    int orig = Item::count;

    Item* b = new Item;

    Item* t = new Item("cervena", 255, 0, 0);
    b->insertFirst(t);
    b->insertFirst(new Item("modra", 0, 0, 255));
    b->insertLast(new Item("zelena", 0, 255, 0));
    t->insertPrev(new Item("zluta", 255, 255, 0));
    t->insertNext(new Item("oranzova", 0xff, 0xa5, 0));

    Item* m = b->search("modra");
    assert(m != nullptr);
    m->insertPrev(new Item("fialova", 255, 0, 255));

    Item* z = b->search("zelena");
    assert(z != nullptr);
    z->insertNext(new Item("hneda", 255, 255, 0));

    // Item s ("svetle zelena", 128, 255, 128);
    // b->insertLast (&s);

    b->print();

    assert(b->length() == 7);

    assert(Item::count == orig + 1 + 7);

    delete b;

    assert(Item::count == orig);
}
#endif

int main()
{
    Item* savci = new Item("savci");

    Item* selmy = new Item("selmy");
    savci->insertLast(selmy);

    Item* medvedovite = new Item("medvedovite");
    selmy->insertLast(medvedovite);

    Item* medved = new Item("hnedy medved", "https://upload.wikimedia.org/wikipedia/commons/thumb/2/2a/Brown_Bear_us_fish.jpg/160px-Brown_Bear_us_fish.jpg");
    medvedovite->insertFirst(medved);

    Item* ledni_medved = new Item("ledni medved", "https://upload.wikimedia.org/wikipedia/commons/thumb/a/af/Medv%C4%9Bd_ledn%C3%AD_%28Ursus_maritimus%29.png/184px-Medv%C4%9Bd_ledn%C3%AD_%28Ursus_maritimus%29.png");
    medvedovite->insertLast(ledni_medved);

    savci->html(cout);

    // #include <fstream>
    ofstream f("output.html");
    savci->html(f);
    f.close();

    cout << "O.K." << endl;
}

