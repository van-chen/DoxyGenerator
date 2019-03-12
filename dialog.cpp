#include "dialog.h"
#include "ui_dialog.h"
#include <QDir>
#include <QSettings>
#include <QDirIterator>
#include <QFileDialog>
#include "filedescription.h"
#include "QTextCodec"
#include "QProcess"
#include "output.h"
#include "qxmlstream.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),DT(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QSettings settings("GIS", "OxyGenerator");
    ui->tbExclude->setText(settings.value("excludes").toString());
    ui->leWorkingDir->setText(settings.value("workingDir").toString());
    ui->leOutputDirectory->setText(settings.value("outputDir").toString());
    ui->leProjectPath->setText(settings.value("projectDir").toString());
    ui->leProjectName->setText(settings.value(ui->leProjectPath->text()).toString());
    ui->teOutput->setText(settings.value(ui->leProjectPath->text()+"out").toString());
    if (!ui->teOutput->toPlainText().isEmpty()) {
        result = ui->teOutput->toPlainText().split("\n", QString::SkipEmptyParts);
        ui->pbSave->setEnabled(true);
        ui->pbDoxyTemplate->setEnabled(true);
        ui->pbGenerateFileDescription->setEnabled(true);
        ui->pbShowWarnings->setEnabled(true);
    }
}

Dialog::~Dialog()
{
    QSettings settings("GIS", "OxyGenerator");
    settings.setValue("excludes", ui->tbExclude->toPlainText());
    settings.setValue("workingDir", ui->leWorkingDir->text());
    settings.setValue("outputDir", ui->leOutputDirectory->text());
    settings.setValue("projectDir", ui->leProjectPath->text());
    settings.setValue(ui->leProjectPath->text(), ui->leProjectName->text());
    settings.setValue(ui->leProjectPath->text()+"out", ui->teOutput->toPlainText());
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    result.clear();
    ui->teOutput->clear();
    auto WD = ui->leWorkingDir->text();
    auto temp = ui->tbHeaders->toPlainText();
    auto headers = temp.split("\n", QString::SkipEmptyParts);
    auto outputDir = ui->leOutputDirectory->text();
    temp = ui->tbExclude->toPlainText();
    auto excludeList = temp.split("\n", QString::SkipEmptyParts);
    ui->progress->setMaximum(headers.size());
    ui->progress->setValue(0);
    ui->pbSave->setEnabled(false);
    ui->pbShowWarnings->setEnabled(false);
    ui->pbDoxyTemplate->setEnabled(false);
    ui->pbGenerateFileDescription->setEnabled(false);


    QMutableListIterator<QString> it(headers);
    int count {0};

    while (it.hasNext()) {
        it.next();

        ui->progress->setValue(++count);
        QApplication::processEvents();

        if (it.value().contains(WD, Qt::CaseInsensitive)) {
            auto filePath = it.value();
            filePath.remove(0, filePath.indexOf(":\\", Qt::CaseInsensitive)-1);
            QFileInfo exists(filePath);
            if (!exists.exists())
                continue;

             filePath = exists.absoluteFilePath();

            bool removed{false};
            for (auto & exclude : excludeList ) {
                if (filePath.contains(QDir::fromNativeSeparators(exclude), Qt::CaseInsensitive)) {
                    it.remove();
                    removed = true;
                    break;
                }
            }
            if (removed)
                continue;

            QDir testDir(outputDir);
            auto relative = testDir.relativeFilePath(filePath);
            if (relative != filePath && !result.contains(relative, Qt::CaseInsensitive)) {
                result.push_back( relative);
                ui->teOutput->append(relative);
                QApplication::processEvents();
            }

        }
        else
            it.remove();
    }

    QFile f(ui->leProjectPath->text());
    if (f.open(QIODevice::ReadOnly|QIODevice::Text)) {
        QXmlStreamReader xml(&f);
        while (!xml.atEnd()) {
            auto token = xml.readNext();
            if (token == QXmlStreamReader::StartElement && xml.name() == "ClCompile") {
                if (xml.attributes().hasAttribute("Include")) {
                    QString name = xml.attributes().value("Include").toString();
                    QFileInfo info(f);
                    auto fileDir = info.dir();
                    info.setFile(fileDir.path() +"\\"+name);

                    bool foundInExclude{false};
                    for (auto & exclude : excludeList ) {
                        if (info.absoluteFilePath().contains(QDir::fromNativeSeparators(exclude), Qt::CaseInsensitive)) {
                            foundInExclude = true;
                            break;
                        }
                    }
                    if (foundInExclude)
                        continue;

                    QDir testDir(outputDir);
                    auto relative = testDir.relativeFilePath(info.absoluteFilePath());
                    if (relative != info.absoluteFilePath() && !result.contains(relative, Qt::CaseInsensitive)) {
                        result.push_back( relative);
                        ui->teOutput->append(relative);
                        QApplication::processEvents();
                    }
                }
            }
        }
    }

    ui->pbSave->setEnabled(true);
    ui->pbDoxyTemplate->setEnabled(true);
    ui->pbGenerateFileDescription->setEnabled(true);
    ui->pbShowWarnings->setEnabled(true);
}

void Dialog::on_pbSave_clicked()
{
    auto temp = PreapreTemplate();

    auto filename = QFileDialog::getSaveFileName(this, "Save oxy", ui->leOutputDirectory->text()+"\\"+ui->leProjectName->text()+".oxy", "*.oxy");
    if (!filename.isEmpty()) {
        QFile f(filename);
        f.open(QIODevice::WriteOnly|QIODevice::Text);
        f.write(temp.toUtf8());
        m_savedFileName = filename;
    }

}

QString Dialog::PreapreTemplate()
{
    auto temp = DT.Template();
    QString headers;
    for (auto it = result.begin(); it != result.end(); ++it) {
        headers.append(*it);
        if (it != result.end()-1)
            headers.append(" \\");
        headers.append("\n");
    }
    temp.replace("<INPUT>", headers);
    temp.replace("<PROJECT_NAME>", ui->leProjectName->text());

    return temp;
}

void Dialog::on_pbDoxyTemplate_clicked()
{
    DT.show();
}

void Dialog::on_leProjectName_textChanged(const QString &arg1)
{


}

void Dialog::on_tbHeaders_textChanged()
{

}


static QString heuristicsСonver(const QByteArray& bytes)
{
	QString result = QString::fromLocal8Bit(bytes);

	//если содержит хотя бы 3 символа которые скорее всего являются кириллицей
	if (3 <= result.count(QRegExp("Р(™|¦|Ј|љ|•|ќ|“|Ё|©|—|Ґ|Є|¤|«|’|ђ|џ| |ћ|›|”|–|­|Ї|§|Ў|њ||ў|¬|‘|®|Ѓ|№|†|ѓ|є|µ|Ѕ|і|€|‰|·|…|Љ|„|‹|І|°|ї|Ђ|ѕ|»|ґ|¶|Ќ|Џ|‡|Ѓ|ј|ё|‚|Њ|±|Ћ|‘)"))) {
		result = QString::fromUtf8(bytes);
	}
	return result;
}



void Dialog::on_pbGenerateFileDescription_clicked()
{
    FileDescription d(this);

    for (auto & it : result) {
            QFile file(ui->leOutputDirectory->text()+"\\"+it);
            QFileInfo fileInfo(ui->leOutputDirectory->text()+"\\"+it);
            auto name = it;
            name.remove("../");
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                bool foundBrief{false};
                while (!file.atEnd()) {
                        auto line = file.readLine();
                        if (name.contains("stdafx", Qt::CaseInsensitive)) {
                            d.AppendText( name + " - Предкомпилируемые заголовки" );
                            foundBrief = true;
                            break;
                        }
                        else
                            if (fileInfo.suffix() == "cs") {
                            if (name.contains(".Designer.cs")) {
                                d.AppendText( name + " - Генерируемый файл формы" );
                                foundBrief = true;
                                break;
                            }
                            else {
                                if (line.contains("/// <summary>")) {
                                    line = file.readLine();
                                    auto t = (name+" - "+ QString::fromUtf8(line).remove("/// "));
                                    d.AppendText( t.remove("\n") );
                                    foundBrief = true;
                                    break;
                                }
                            }
                        }
							else {
								if (line.contains("\\file") || line.contains("@file")) {

									QString description;
									for (int i = 0; i < 5; i++) {
										QString line = heuristicsСonver(file.readLine());
										auto pos = line.indexOf("brief");
										if (pos != -1) {
											description = line.remove(0, pos + 5).simplified();
											break;
										}

										if (line.contains("*/")) {
											break;
										}
										description += line + " ";
									}
									description = description.trimmed();
									if (!description.isEmpty()) {

										foundBrief = true;
										auto t = (name + " - " + description);
										d.AppendText(t.remove("\n"));
										break;
									}
								}
							}
				}
				if ( !foundBrief)
					d.AppendText(name+" - "+"Описание не обнаружено");
			}
			else {
				d.AppendText(name+" - "+file.errorString());
			}
    }

    d.exec();
}

void Dialog::on_pbSelectProjectDir_clicked()
{
    ui->leProjectPath->setText( QDir::toNativeSeparators( QFileDialog::getOpenFileName(this, "", ui->leProjectPath->text(), "*.vcxproj") ) );
    if (!ui->leProjectPath->text().isEmpty()) {

        QSettings settings("GIS", "OxyGenerator");
        ui->leProjectName->setText(settings.value(ui->leProjectPath->text()).toString());
        ui->teOutput->setText(settings.value(ui->leProjectPath->text()+"out").toString());

        if (!ui->teOutput->toPlainText().isEmpty()) {
            result = ui->teOutput->toPlainText().split("\n", QString::SkipEmptyParts);
            ui->pbSave->setEnabled(true);
            ui->pbDoxyTemplate->setEnabled(true);
            ui->pbGenerateFileDescription->setEnabled(true);
            ui->pbShowWarnings->setEnabled(true);
        }

        if (ui->leProjectName->text().isEmpty()) {
            QFileInfo info(ui->leProjectPath->text());
            ui->leProjectName->setText(info.fileName().section('.', 0, 0));
        }


    }
}

void Dialog::on_pbSelectSolutionDir_clicked()
{
    ui->leWorkingDir->setText( QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, "", ui->leWorkingDir->text())) );
}

void Dialog::on_pbSelectDocDir_clicked()
{
    ui->leOutputDirectory->setText( QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, "", ui->leOutputDirectory->text())) );
}

void Dialog::on_pbShowWarnings_clicked()
{
    auto temp = PreapreTemplate();

        QFile f(ui->leOutputDirectory->text()+"\\temp.oxy");
        f.open(QIODevice::WriteOnly|QIODevice::Text);
        f.write(temp.toUtf8());
        f.flush();

    QProcess process;
        process.setWorkingDirectory(ui->leOutputDirectory->text());
        auto relativeFilePath = QDir(process.workingDirectory()).relativeFilePath(f.fileName());
        auto toStart = QString("doxygen \"%1\" ").arg(QDir::toNativeSeparators( relativeFilePath ));
        process.start(toStart);

        QTextCodec *codec = QTextCodec::codecForLocale();

        Output O(this);
        if (process.waitForStarted() && process.waitForFinished() && process.exitCode() == 0) {
            QByteArray ba = process.readAllStandardOutput();
            O.SetLog(codec->toUnicode(ba));

            ba = process.readAllStandardError();
            O.SetWarnings(codec->toUnicode(ba));
        }
        else {
            QByteArray ba = process.readAllStandardError();
            QString err = codec->toUnicode(ba);
            O.SetLog(err);
        }
        O.exec();

}
