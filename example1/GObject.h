#pragma once

#include <QObject>

class GObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(QString word READ word WRITE setWord NOTIFY wordChanged)

public:
    /**
     * Q_INVOKABLE!
     */
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
    static ObjectFactory& instance()
    {
        static ObjectFactory factory;
        return factory;
    }

    template <Derived<QObject> T>
    void registerClass()
    {
        if (const auto meta = T::staticMetaObject; !_map.contains(meta.className())) {
            _map[meta.className()] = meta;
        }
    }

    std::unique_ptr<QObject> createInstance(const QString& className) const
    {
        if (!_map.contains(className))
            return {};

        std::unique_ptr<QObject> ptr(_map.at(className).newInstance());
        return ptr;
    }

private:
    ObjectFactory() = default;

private:
    std::unordered_map<QString, QMetaObject> _map;
};
