#include <string>
#include <map>

class Type {
public:
    Type() = default;
    Type(std::string name);
    Type(const Type & other) = default;
    std::string getName(void);
    void dump(void);
private:
    std::string _name;
};

class Registry {
public:
    Registry() = default;
    Registry(const Registry &) = delete;
    ~Registry() = default;

    void registerType(Type t);
    
    void dump();
private:
    std::map<std::string, Type> _typeMap;
};


class Scope {
public:
    Scope();
    ~Scope() = default;

    void inheritScope(Scope *other);

    /**
     * Causes this scope to resolve
     * a specific definition that may not
     * have been exported anywhere.
     */
    void defineType(int type);

    /**
     * Causes this scope to resolve
     * definitions from an exported
     * registry.  For example, a program
     * may import a global registry
     * and then a graph may inherit the
     * scope of the program.
     */
    void importDefs(int registry);

    std::map<std::string, Type> _typeMap;
};

/****************
 Scopes:
    Program scope:
        This scope is global to the program and every graph in the program has
        access to these scopes.  This is the parent scope of every graph.
    Graph scope:
        Only types defined in this scope or below may see them.

Should we define scope operations algebraicly?  i.e. adding a graph should be invariant of order?

s(g1,g2) = s(g2,g1)

s(g1(g2), g3) = s(g1, g3) for all g3.  This means that g2 cannot create a new scope outside itself.


 */
