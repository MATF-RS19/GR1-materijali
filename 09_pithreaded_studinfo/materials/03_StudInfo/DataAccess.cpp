#include "DataAccess.h"

#include <QFile>
#include <QTextStream>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

DataAccess::DataAccess(Ui::FormMain * form)
    : ui(form)
{
}



void DataAccess::read(const QString & file)
{
    if (file.endsWith(".xml")) {
        readXML(file);
    } else {
        readText(file);
    }
}

void DataAccess::write(const QString & file)
{
    if (file.endsWith(".xml")) {
        writeXML(file);
    } else {
        writeText(file);
    }
}

// "plaintext" citanje i pisanje podataka

void DataAccess::readText(const QString & file)
{
    QFile data(file);
    if (data.open(QFile::ReadOnly)) {
        QTextStream in(&data);

        QString value;

        // Ucitavamo osnovne podatke o studentu

        value = in.readLine().trimmed();  ui->textName->setText(value);
        value = in.readLine().trimmed();  ui->textSurname->setText(value);
        value = in.readLine().trimmed();  ui->textID->setText(value);
        value = in.readLine().trimmed();  ui->textJMBG->setText(value);

        // Da li je student upisan?

        value = in.readLine().trimmed();

        ui->radioEnrolled->setChecked(value == "1");
        ui->radioNotEnrolled->setChecked(value == "0");

        // Ako jeste, ucitavamo spisak prijavljenih ispita

        if (value == "1") {
            // Cistimo spisak prethodno prijavljenih ispita
            ui->listExams->clear();

            // Citamo liniju po liniju dok ne naletimo na kraj
            for(;;) {
                value = in.readLine();

                if (!value.isNull()) {
                    ui->listExams->addItem(value);
                } else {
                    break;
                }

            }
        }
    }

    data.close();
}

void DataAccess::writeText(const QString & file)
{
    QFile data(file);
    if (data.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&data);

        // Ucitavamo osnovne podatke o studentu

        out << ui->textName->text() << '\n';
        out << ui->textSurname->text() << '\n';
        out << ui->textID->text() << '\n';
        out << ui->textJMBG->text() << '\n';

        // Da li je student upisan?

        out << (ui->radioEnrolled->isChecked() ? '1' : '0') << '\n';

        // Ako jeste, ispisujemo spisak prijavljenih ispita

        if (ui->radioEnrolled->isChecked()) {
            for (int i = 0; i < ui->listExams->count(); i++) {
                out << ui->listExams->item(i)->text() << '\n';
            }
        }
    }

    data.close();
}

// Citanje i pisanje podataka u XML formatu

void DataAccess::readXML(const QString & file)
{
    // Otvaramo fajl
    QFile data(file);
    if (!data.open(QFile::ReadOnly))
        return;

    // Kreiramo ulazni XML tok nad fajlom
    QXmlStreamReader xml(&data);

    // Kad naletimo na pocetni XML tag, proveravamo koji je
    // i ucitavamo podatke
    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isStartElement()) {
            if (xml.name() == "name") {
                ui->textName->setText(
                        xml.readElementText());
            }
            else if (xml.name() == "surname") {
                ui->textSurname->setText(xml.readElementText());
            }
            else if (xml.name() == "id") {
                ui->textID->setText(xml.readElementText());
            }
            else if (xml.name() == "jmbg") {
                ui->textJMBG->setText(xml.readElementText());
            }
            else if (xml.name() == "enrolled") {
                QString value = xml.readElementText();
                ui->radioEnrolled->setChecked(value == "1");
                ui->radioNotEnrolled->setChecked(value == "0");
            }
            else if (xml.name() == "exam") {
                ui->listExams->addItem(xml.readElementText());
            }
        }
    }

    data.close();
}

void DataAccess::writeXML(const QString & file)
{
    // Otvaramo fajl za pisanje
    QFile data(file);
    if (!data.open(QFile::WriteOnly | QFile::Truncate))
        return;

    // Otvaramo izlazni XML tok nad fajlom
    QXmlStreamWriter xml(&data);

    // Ukljucujemo formatiranje XML-a - da ne bude ceo
    // u jednom redu
    xml.setAutoFormatting(true);

    // Pocinjemo dokument i ispisujemo koreni element
    xml.writeStartDocument();
    xml.writeStartElement("student");

    // Ispisujemo podatke
    xml.writeTextElement("name", ui->textName->text());
    xml.writeTextElement("surname", ui->textSurname->text());
    xml.writeTextElement("id", ui->textID->text());
    xml.writeTextElement("jmbg", ui->textJMBG->text());

    xml.writeTextElement("enrolled",
        (ui->radioEnrolled->isChecked() ? "1" : "0"));

    // Prijavljene ispite ispisujemo u pod-tagu <exams>
    if (ui->radioEnrolled->isChecked()) {
        xml.writeStartElement("exams");
        for (int i = 0; i < ui->listExams->count(); i++) {
            xml.writeTextElement("exam", ui->listExams->item(i)->text());
        }
        xml.writeEndElement();
    }

    // Zatvaramo koreni element i sam XML dokument
    xml.writeEndElement();
    xml.writeEndDocument();

    data.close();
}

