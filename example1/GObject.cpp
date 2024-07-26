#include "GObject.h"

#include "moc_GObject.cpp"

/**
 * IMPORTANT: _register must be called after #include "moc_GObject.cpp"!
 */
static bool _register = ([]() { ObjectFactory::instance().registerClass<GObject>(); }(), true);
