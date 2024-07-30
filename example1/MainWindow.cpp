#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QMetaProperty>

#include "GObject.h"
#include "MainWindow.h"
#include "./ui_MainWindow.h"

namespace {
const QString kExampleJsonPath = QStringLiteral("D:/projects/qt_learning/example1/data.json");
const QString kClassName = QStringLiteral("className");
const QString kProperties = QStringLiteral("properties");

QJsonDocument loadJson(const QString& path)
{
    QFile loadFile(path);
    if (!loadFile.open(QIODevice::ReadOnly))
        return {};

    return QJsonDocument::fromJson(loadFile.readAll());
}
} // namespace

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1. Create instance from Json file
    const QJsonObject jo = loadJson(kExampleJsonPath).object();
    auto obj = ObjectFactory::instance().createInstance(jo.value(kClassName).toString());

    // 2. Construct UI according to properies
    for (const auto ja = jo[kProperties].toArray(); const auto& jv : ja) {
        const auto propertyName = jv.toString();
        const auto propertyVal = jo.value(propertyName);
        obj->setProperty(propertyName.toLocal8Bit().data(), propertyVal);
    }
    qDebug() << obj->value() << obj->word();
}

MainWindow::~MainWindow() { delete ui; }
