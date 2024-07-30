#include "GObject.h"

#include "moc_GObject.cpp"

/**
 * IMPORTANT: _register must be called after #include "moc_GObject.cpp"!
 */
static auto _register = []() { return ObjectFactory::instance().registerClass<GObject>(); }();
