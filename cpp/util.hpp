#ifndef UTIL_HPP
#define UTIL_HPP

#include <cassert>
#include <array>

///
/// \brief The non_nullptr class
///
/// This class ensure that its pointer is never nullptr
/// \example non_nullptr<int> intPtr(new int);
///

template<class T>
class non_nullptr
{
public:
    using pointer = T*;
    using element_type = T;

    non_nullptr(decltype(nullptr)) = delete;
    non_nullptr(int) = delete;

    non_nullptr& operator = (decltype(nullptr)) noexcept = delete;
    non_nullptr& operator = (int) noexcept = delete;

    non_nullptr(T* ptr) noexcept
    : m_ptr(ptr)
    {
        assert(ptr);
    }

    non_nullptr& operator = (T* ptr) noexcept
    {
        assert(ptr);
        this->m_ptr = ptr;
        return *this;
    }

    bool operator==(const non_nullptr<T>& other) const
    {
        return m_ptr == other.m_ptr;
    }

    T* get() const noexcept { return m_ptr; }
    T& operator*() const noexcept { return *m_ptr; }
    T* operator->() const noexcept { return m_ptr; }

private:
    T* m_ptr;
};

class Def
{
public:
    static const unsigned MAPX          {15u}; //For arrays.
    static const unsigned MAPY          {8u};

    static const unsigned TILEARRAYSIZE {MAPX*MAPY};
    using  TILEARRAY    = std::array    <unsigned, TILEARRAYSIZE>;

    static const unsigned MAPXMAP       {14u}; //For logic
    static const unsigned MAPYMAP       {14u};

    static const unsigned MAPXSIZE      {512u};
    static const unsigned MAPYSIZE      {256u};

    static const unsigned BUTTONSXSIZE  {MAPXSIZE};
    static const unsigned BUTTONSYSIZE  {90u};

    static const unsigned TILEXSIZE     {64u};
    static const unsigned TILEYSIZE     {32u};

    static const unsigned TILEGXSIZE    {32u};
    static const unsigned TILEGYSIZE    {16u};

    static const unsigned MAXPOSINTILE  {4u};
};

inline unsigned dToU(double d)
{
    return static_cast<unsigned>(d);
}

template<class T>
T distance(const T& x, const T& y)
{
    return (x > y) ? x - y : y - x;
}

using AbsTile = std::pair<unsigned, unsigned>; // Absolute Tile (0-15, 0-8)
using DiffTile = std::pair<int, int>; //Difference Tile (can be negative or positive)

/*
Les idées utiles:
Pour les items & cie: Faire une factory au lieu d'entités d'exemples.
factory(Entity& e, string itemtomake) { ... }

Pour communication inter-systems:
EVENT QUEUE. Direct ! http://gameprogrammingpatterns.com/event-queue.html
En gros:
Dans la loop:
EventQueue q;
Event* e = q.getNextEvent();
LesSystems.notify(e);

Aussi, dans la func qui handle l'event, NE PAS FAIRE L'ACTION DIRECTEMENT ! Utiliser une event queue interne.
ex:
void Audio::playSound(const Event& e)
if (!maxpending)
	m_pending.push_back(...)
Comme ça c'est hyper rapide, et pas de problème !
Après, une func update() et voilà.

Mais... ça sera pas rapide ! à chaque delete du premier obj du vec, on doit avancer tous les objs !
Donc, faire "pattern" head tail.
playsound()
on ajoute à la pos de tail.
update()
on utilise array[head]
Quand on a fini, ++head
(voir gpp, je sais pas expliquer)

Animation System:
Mettre code qui change l'image dans le system.
De plus, pas 100000 images, 1 images avec toutes les images. Une texture c'est lourd putain !
AI System: Doit calculer un chemin et le mettre dans un composant path selon la pos et la diff.
Movement: Doit suivre ce simple chemin. actualiser pos & chemin
*/

#endif // UTIL_HPP
