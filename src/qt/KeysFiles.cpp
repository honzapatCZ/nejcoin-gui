// Copyright (c) 2019-2019, Nejcraft
// Copyright (c) 2014-2019, The Monero Project
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QMap>
#include <QDebug>
#include <QUrl>
#include <QtConcurrent/QtConcurrent>
#include <QMutex>
#include <QMutexLocker>
#include <QString>
#include "src/libwalletqt/WalletManager.h"
#include "src/NetworkType.h"
#include "src/qt/utils.h"

#include "KeysFiles.h"


WalletKeysFiles::WalletKeysFiles(const qint64 &modified, const qint64 &created, const QString &path, const quint8 &networkType, const QString &address)
    : m_modified(modified), m_created(created), m_path(path), m_networkType(networkType), m_address(address)
{
}

qint64 WalletKeysFiles::modified() const
{
    return m_modified;
}

QString WalletKeysFiles::address() const
{
    return m_address;
}

qint64 WalletKeysFiles::created() const
{
    return m_created;
}

QString WalletKeysFiles::path() const
{
    return m_path;
}

quint8 WalletKeysFiles::networkType() const
{
    return m_networkType;
}


WalletKeysFilesModel::WalletKeysFilesModel(WalletManager *walletManager, QObject *parent)
    : QAbstractListModel(parent)
{
    this->m_walletManager = walletManager;
    this->m_walletKeysFilesItemModel = qobject_cast<QAbstractItemModel *>(this);

    this->m_walletKeysFilesModelProxy.setSourceModel(this->m_walletKeysFilesItemModel);
    this->m_walletKeysFilesModelProxy.setSortRole(WalletKeysFilesModel::ModifiedRole);
    this->m_walletKeysFilesModelProxy.setDynamicSortFilter(true);
    this->m_walletKeysFilesModelProxy.sort(0, Qt::DescendingOrder);
}

QSortFilterProxyModel &WalletKeysFilesModel::proxyModel()
{
    return m_walletKeysFilesModelProxy;
}

void WalletKeysFilesModel::clear()
{
    beginResetModel();
    m_walletKeyFiles.clear();
    endResetModel();
}

void WalletKeysFilesModel::refresh(const QString &nejcoinAccountsDir)
{
    this->clear();
    this->findWallets(nejcoinAccountsDir);
}

void WalletKeysFilesModel::findWallets(const QString &nejcoinAccountsDir)
{
    QStringList walletDir = this->m_walletManager->findWallets(nejcoinAccountsDir);
    foreach(QString wallet, walletDir){
        if(!fileExists(wallet + ".keys"))
            continue;

        quint8 networkType = NetworkType::MAINNET;
        QString address = QString("");

        // attempt to retreive wallet address
        if(fileExists(wallet + ".address.txt")){
            QFile file(wallet + ".address.txt");
            file.open(QFile::ReadOnly | QFile::Text);
            QString _address = QTextCodec::codecForMib(106)->toUnicode(file.readAll());

            if(!_address.isEmpty()){
                address = _address;
                if(address.startsWith("5") || address.startsWith("7")){
                    networkType = NetworkType::STAGENET;
                } else if(address.startsWith("9") || address.startsWith("B")){
                    networkType = NetworkType::TESTNET;
                }
            }

            file.close();
        }

        const QFileInfo info(wallet);
        const QDateTime modifiedAt = info.lastModified();
        const QDateTime createdAt = info.created();  // @TODO: QFileInfo::birthTime() >= Qt 5.10

        this->addWalletKeysFile(WalletKeysFiles(modifiedAt.toSecsSinceEpoch(),
                                                createdAt.toSecsSinceEpoch(),
                                                info.absoluteFilePath(), networkType, address));
    }
}

void WalletKeysFilesModel::addWalletKeysFile(const WalletKeysFiles &walletKeysFile)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_walletKeyFiles << walletKeysFile;
    endInsertRows();
}

int WalletKeysFilesModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_walletKeyFiles.count();
}

QVariant WalletKeysFilesModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_walletKeyFiles.count())
        return QVariant();

    const WalletKeysFiles &walletKeyFile = m_walletKeyFiles[index.row()];
    if (role == ModifiedRole)
        return walletKeyFile.modified();
    else if (role == PathRole)
        return walletKeyFile.path();
    else if (role == NetworkTypeRole)
        return walletKeyFile.networkType();
    else if (role == AddressRole)
        return walletKeyFile.address();
    else if (role == CreatedRole)
        return walletKeyFile.created();
    return QVariant();
}

QHash<int, QByteArray> WalletKeysFilesModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[ModifiedRole] = "modified";
    roles[PathRole] = "path";
    roles[NetworkTypeRole] = "networktype";
    roles[AddressRole] = "address";
    roles[CreatedRole] = "created";
    return roles;
}
