/* 
 * File:   SMModel.cpp
 * Author: Alexander Keller, Robert Heimsoth, Thomas Matern
 *
 * HS BREMEN, SS2014, TI6.2
 */

#include "SMModel.h"
#include <assert.h>
#include <string>
#include <bitset>
#include <cstdlib>
#include <limits.h>
#include <regex>
#include <iostream>
#include <cstring>

using namespace std;

/**
 * Modifiziert die Bytes für das enkodierte Bild so,
 * dass in den letzten Bits eines jeden Bytes des Bildes
 * die übergeben Nachricht gespeichert wird.
 * 
 * @param msg In das Bild einzufügende Nachricht
 */
void SMModel::encode(const string& msg) {

    // Nachrichtenspezifischen Header und Nachricht in encoded_msg speichern
    string encoded_msg(createHeader(msg) + msg);
    // Länge der gesamten Nachricht (inkl. Header) speichern
    unsigned int encoded_msg_size = encoded_msg.size();

    // Speicherplatz für die Binärrepräsentation der Nachricht reservieren
    // Ein Zeichen der Nachricht benötigt 8 Bit
    char* binary = new char[encoded_msg_size * 8];

    size_t byte_counter = 0;
    // Es wird jedes einzelne Zeichen des Strings genommen...
    for (size_t i = 0; i < encoded_msg_size; i++) {
        // ...und die Bitrepräsentation des ASCII Wertes des Zeichens 
        // in einer temporären Variable gespeichert
        string byte = charToBits(encoded_msg.at(i));

        // Die 8 Nullen / Einsen werden in binary für die Bitrepräsentation
        // des Bildes gespeichert.
        for (size_t j = 0; j < 8; j++) {
            binary[byte_counter + j] = byte.at(j);
        }
        // Um 8 erhöhen, damit beim nächsten Schleifendurchlauf
        // nicht bereits vorhandene Daten in binary überschrieben werden
        byte_counter += 8;
    }

    // Reservieren von neuem Speicherplatz für das modizifierte Bild
    // in der Größe des unmodifizierten Bildes und ebenfalls Übernehmen der Länge
    modCarrierBytes = new unsigned char[unmodCarrierBytesLength];
    modCarrierBytesLength = unmodCarrierBytesLength;

    // Das unmodifizierte Bild Bit für Bit in das modifizierte Bild kopieren
    for (size_t i = 0; i < unmodCarrierBytesLength; i++) {
        modCarrierBytes[i] = unmodCarrierBytes[i];
    }
    // Speichern der Bits der Nachricht in das modifizierte Bild
    // Dazu Nachrichtenlänge * 8, da jedes Zeichen 8 Bits benötigt
    for (size_t i = 0; i < encoded_msg_size * 8; i++) {
        // Das letzte Bit des i-ten Bytes des Bildes auf 0 setzen,
        // indem mit dem Muster 11111110 (dez. 254) verundet wird
        modCarrierBytes[i] = modCarrierBytes[i] & 254;

        // Sollte an der Stelle in der Binärrepräsentation der Nachricht eine 1
        // stehen, dann diese 1 in dem Bild an der letzten Stelle speichern.
        // Dazu wird das Byte mit dem Muster 00000001 (dez. 1) verodert
        if (binary[i] == '1') {
            modCarrierBytes[i] = modCarrierBytes[i] | 1;
        }
    }
}

/**
 * Methode zum ermitteln der Länge des Headers
 * 
 * @return Länge des Headers
 */
unsigned int SMModel::getHeaderSize() {
    // Headerlänge setzt sich zusammen aus der Signaturlänge
    // und 4 Bytes für das Speichern der Nachrichtenlänge
    return (SGN.size() + 4);
}

/**
 * Holt eine versteckte Nachricht, welche in einem Bild als Bitrepräsentation
 * vorhanden ist.
 * @return Versteckte Nachricht
 */
string SMModel::decode() {

    // Leeren String für das Speichern Bitrepräsentation der Länge der Nachricht
    string msg_binary_size("");

    // Der Offset von der Signatur, da die Länge im Header direkt
    // nach der Signatur gespeichert ist
    size_t msgSizeOffset = SGN.size() * 8;

    // Die vier Bytes der Länge rauslesen
    for (size_t i = 0; i < 4; i++) {
        // Pro Byte wiederum jedes Bit raussuchen
        for (size_t j = 0; j < 8; j++) {
            // Die einzelnen Bits des ersten Bytes speichern
            // indem das letzte Bit des Bytes des kodierten Bildes gespeichert wird
            msg_binary_size += charToBits(modCarrierBytes[msgSizeOffset + i * 8 + j]).at(7);
        }
    }
    // Die Nachrichtenlänge, welche als Binärrepräsentation als String vorliegt
    // in ein bitset umformen, um daraus wiederum eine Dezimalzahl zu erzeugen
    size_t msg_size = bitset < 32 > (msg_binary_size).to_ulong();

    // Speicherplatz für die Bitrepräsentation der Nachricht reservieren
    // (Nachrichtenlänge * 8, da ein Zeichen ein Byte = 8 Bit groß ist)
    char* binary = new char[msg_size * 8];

    size_t bit_counter = 0;

    // Da die Nachricht erst nach dem Header anfängt, ist das erste Bit, welches
    // zur Nachricht gehört direkt nach dem Header. Deshalb ist der Offset,
    // nachdem die Nachricht anfängt Headergröße * 8.
    size_t header_offset = getHeaderSize()*8;

    // Die Nachricht aus dem kodierten Bild holen
    // Beginn der Schleife nach dem Ende des Headers, Ende nach dem Ende der
    // aus dem Bild geholten Nachrichtenlänge
    for (size_t i = header_offset; i < header_offset + msg_size * 8; i++) {
        //Das letzte Bit des i-ten Bytes des modifzierten Bildes für die
        //Bitrepräsentation speichern
        binary[bit_counter] = charToBits(modCarrierBytes[i]).at(7);
        bit_counter++;
    }

    // String für die dekodierte Nachricht anlegen
    string decoded_msg("");

    // String für die Zwischenspeicherung der 
    // Bitrepräsenation eines Zeichens der Nachricht anlegen
    string decoded_msg_buffer("");

    bit_counter = 0;
    // Über jedes Bit der Nachricht gehen
    for (size_t i = 0; i < msg_size * 8; i++) {
        // Speichere einzelne Bits eines Zeichens der Nachricht
        decoded_msg_buffer += binary[i];
        bit_counter++;

        // Sofern ein Zeichen vollständig ist (8 Bits erreicht)...
        if (bit_counter == 8) {
            // Zeichen dem String für die dekodierte Nachricht hinzufügen
            decoded_msg += bitsToChar(decoded_msg_buffer);
            // Zwischenspeicher für die Bitrepräsentation eines Zeichens zurücksetzen
            decoded_msg_buffer = "";
            bit_counter = 0;
        }

    }
    return decoded_msg;
}

/**
 * Prüft ob das modifizierte Bild eine Header Signature enthält 
 * und damit eine Nachricht
 * 
 * @return true, wenn Signatur gefunden, false wenn nicht
 */
bool SMModel::checkForHeaderSignature() const {

    // Prüfe Übereinstimmung mit einzelnen Signatur Zeichen 
    // mit den ersten Zeichen der Bitmap
    for (size_t i = 0; i < SGN.length(); i++) {

        // String zur Zwischenspeicherung der Bitrepräsentation eines Bytes im Bild
        string modCarrierBytesAsBit("");

        // Da ein Zeichen 8 Bits enthält, muss für das nächste Zeichen 8 Bits 
        // weiter gesucht werden.
        size_t offset = i * 8;
        // Alle Bits eines Zeichens durchgehen und die Bitrepräsentation speichern
        for (size_t j = 0; j < 8; j++) {
            modCarrierBytesAsBit += charToBits(modCarrierBytes[offset + j]).at(7);
        }

        // Wenn das i-te Zeichen der Signatur nicht dem Zeichen im Bild entspricht,
        // dann kein gültiger Header -> false
        if (SGN[i] != bitsToChar(modCarrierBytesAsBit)) {
            return false;
        }
    }
    return true;
}

/**
 * Liefert das Bitmuster des modifizierten Bildes
 * 
 * @return Bitmuster des modifizierten Bildes
 */
string SMModel::getModBitPattern() {
    string result("");
    // Erstelle für jedes Byte des Bildes die Bitrepräsenation und füge
    // diese dem Ergebnisstring hinzu.
    // (Derzeit begrenzt auf 1000 Zeichen, da sonst Berechnungsdauer 
    // bei großem Bild sehr lang)
    for (size_t i = 0; i < 1000 && i < modCarrierBytesLength; i++) {
        result += charToBits(modCarrierBytes[i]) + " ";
    }
    return result;
}

/**
 * Erstellt einen Header für eine Nachricht
 * 
 * @param msg Nachricht für den ein Header erstellt werden soll
 * @return Den erstellten Header für die Nachricht
 */
string SMModel::createHeader(const std::string& msg) {

    //Header beginnt mit der Signatur
    string header(SGN);

    //Die dezimale Nachrichtenlänge in ein 32-Bit Muster übertragen
    bitset < 32 > bits(msg.size());
    //Das 32-Bit Muster als String zur weiteren Verwendung speichern
    string strBits = bits.to_string();

    // Länge der Nachricht in den Header speichern (4 Byte lang)
    for (size_t i = 0; i < 4; i++) {
        //ASCII Wert der jeweiligen 8 Bits dem Header hinzufügen
        header += bitsToChar(strBits.substr(i * 8, 8));
    }
    return header;
}

/**
 * Gibt eine Bitrepräsentation des ASCII Wertes eines Zeichens als String zurück
 * Etwa: 'A' entspricht "01000001"
 *  
 * @param c Zeichen, dessen Bitrepräsentation erstellt werden soll
 * @return Bitrepräsentation des ASCII Wertes
 */
string SMModel::charToBits(const unsigned char& c) const {
    string bits("");
    for (int i = 7; i >= 0; i--) {
        //Einzelne Bits des Zeichens von rechts in die Bitrepräsentation speichern
        //da append am Ende des Strings etwa anfügt (deshalb von 7->0 und nicht 0->7)
        bits.append((c & (1 << i)) ? "1" : "0");
    }
    return bits;
};

/**
 * Gibt das zugehörige Zeichen einer Bitrepräsentation zurück
 * Etwa: "01000001" entspricht 'A'
 * 
 * @param bits Die Bitrepräsentation des Zeichens
 * @return Das Zeichen, welches der Bitrepräsentation entspricht
 */
unsigned char SMModel::bitsToChar(const std::string& bits) const {
    //Prüfen, ob das Bitmuster 8 Bits lang ist (1 Byte = 1 Zeichen)
    assert(bits.size() == 8);

    //Die Bitrepräsentation des Zeichens als 8-Bit bitset speichern
    std::bitset < 8 > bitsArray(bits);

    //Die Bitrepräsentation als Dezimalzahl speichern
    unsigned int asciiPos = bitsArray.to_ulong();

    //Den ASCII Dezimalwert zu dem Zeichen casten und zurückgeben
    return static_cast<unsigned char> (asciiPos);
};

/**
 * Gibt das Bytemuster des modifizierten Bildes zurück
 * 
 * @return Bytemuster des modiziferten Bildes
 */
unsigned char* SMModel::getModCarrierBytes() const {
    return modCarrierBytes;
};

/**
 * Gibt die Länge des Bytemusters des modifierten Bildes zurück
 * 
 * @return Länge des Bytemusters des modiziferten Bildes
 */
size_t SMModel::getModCarrierBytesLength() const {
    return modCarrierBytesLength;
};

/**
 * Setzt das Bytemuster des modifizierten Bildes auf das übergeben Bytemuster
 * 
 * @param modBytes Bytemuster, welches auf das Bytemuster des modifizierten Bildes gesetzt wird
 * @param len Länge des Bytemusters
 */
void SMModel::setModCarrierBytes(unsigned char* modBytes, size_t len) {

    // Reservieren von neuem Speicherplatz in der Größe der übergebenen Länge
    modCarrierBytes = new unsigned char[len];
    modCarrierBytesLength = len;

    // Das übergebene Bytemuster Byte für Byte in das Bytemuster für das
    // modifizierte Bild kopieren
    for (size_t i = 0; i < len; i++) {
        modCarrierBytes[i] = modBytes[i];
    }
};

/**
 * Gibt das Bytemuster des unmodifizierten Bildes zurück
 * 
 * @return Bytemuster des unmodiziferten Bildes
 */
unsigned char* SMModel::getUnmodCarrierBytes() const {
    return unmodCarrierBytes;
};

/**
 * Gibt die Länge des Bytemusters des unmodifierten Bildes zurück
 * 
 * @return Länge des Bytemusters des unmodiziferten Bildes
 */
size_t SMModel::getUnmodCarrierBytesLength() const {
    return unmodCarrierBytesLength;
};

/**
 * Setzt das Bytemuster des unmodifizierten Bildes auf das übergeben Bytemuster
 * 
 * @param modBytes Bytemuster, welches auf das Bytemuster des unmodifizierten Bildes gesetzt wird
 * @param len Länge des Bytemusters
 */
void SMModel::setUnmodCarrierBytes(unsigned char* unmodBytes, size_t len) {

    // Reservieren von neuem Speicherplatz in der Größe der übergebenen Länge
    unmodCarrierBytes = new unsigned char[len];
    unmodCarrierBytesLength = len;

    // Das übergebene Bytemuster Byte für Byte in das Bytemuster für das
    // unmodifizierte Bild kopieren
    for (size_t i = 0; i < len; i++) {
        unmodCarrierBytes[i] = unmodBytes[i];
    }
};

/**
 * Setzt die modifizierten Bytes zurück.
 */
void SMModel::resetModCarrierBytes() {
    modCarrierBytes = emptyCharArray;
    modCarrierBytesLength = 0;
}

/**
 * Setzt die noch nicht modifizierten Bytes zurück.
 */
void SMModel::resetUnmodCarrierBytes() {
    unmodCarrierBytes = emptyCharArray;
    unmodCarrierBytesLength = 0;
}
