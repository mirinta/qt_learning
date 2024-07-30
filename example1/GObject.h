#pragma once

#include <QObject>

class GObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(QString word READ word WRITE setWord NOTIFY wordChanged)

public:
    Q_INVOKABLE explicit GObject(QObject* parent = nullptr) : QObject(parent){};

    int value() const { return _value; }
    void setValue(int value)
    {
        if (value != _value) {
            _value = value;
            emit valueChanged(_value);
        }
    }

    QString word() const { return _word; }
    void setWord(const QString& word)
    {
        if (word != _word) {
            _word = word;
            emit wordChanged(_word);
        }
    }

signals:
    void valueChanged(int newValue);
    void wordChanged(const QString& newWord);

private:
    int _value{0};
    QString _word;
};

template <typename _Base, typename _Derived>
concept Derived = std::is_base_of_v<_Derived, _Base>;

class ObjectFactory
{
    Q_DISABLE_COPY_MOVE(ObjectFactory);

public:
    ~ObjectFactory() = default;

    static ObjectFactory& instance()
    {
        static ObjectFactory factory;
        return factory;
    }

    template <Derived<QObject> T>
    bool registerClass()
    {
        const auto meta = T::staticMetaObject;
        if (_map.contains(meta.className()))
            return false;

        _map[meta.className()] = meta;
        return true;
    }

    std::unique_ptr<GObject> createInstance(const QString& className) const
    {
        if (!_map.contains(className))
            return {};

        return std::make_unique<GObject>(static_cast<GObject*>(_map.at(className).newInstance()));
    }

private:
    ObjectFactory() = default;

private:
    std::unordered_map<QString, QMetaObject> _map;
};
