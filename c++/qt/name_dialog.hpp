/*
 * rscraper Copyright (C) 2019 Adam Gray
 * This program is licensed with GPLv3.0 and comes with absolutely no warranty.
 * This code may be copied, modified, distributed etc. with accordance to the GPLv3.0 license (a copy of which is in the root project directory) under the following conditions:
 *     This copyright notice must be included at the beginning of any copied/modified file originating from this project, or at the beginning of any section of code that originates from this project.
 */

#pragma once

#include <QDialog>
#include <QLineEdit>


class NameDialog : public QDialog {
  public:
	NameDialog(const QString& title,  const QString& str,  QWidget* parent = 0);
	QLineEdit* name_edit;
};
