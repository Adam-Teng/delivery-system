// delivery.cpp: 定义应用程序的入口点。

#include "include/UI/cli.h"
#include "include/data.h"
#include <QApplication>
#include <memory>

std::shared_ptr<Data> Data::instance(new Data(), Data::destroy);

int main() {
  shared_ptr<Data> d = Data::getInstance();
  d->init("../../../file/user.json", "../../../file/delivery.json");
  new CLI;
  return 0;
}
