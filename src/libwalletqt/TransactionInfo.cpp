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

#include "TransactionInfo.h"
#include "WalletManager.h"
#include "Transfer.h"
#include <QDateTime>
#include <QDebug>

TransactionInfo::Direction TransactionInfo::direction() const
{
    return static_cast<Direction>(m_pimpl->direction());
}

bool TransactionInfo::isPending() const
{
    return m_pimpl->isPending();
}

bool TransactionInfo::isFailed() const
{
    return m_pimpl->isFailed();
}


double TransactionInfo::amount() const
{
    // there's no unsigned uint64 for JS, so better use double
    return WalletManager::instance()->displayAmount(m_pimpl->amount()).toDouble();
}

quint64 TransactionInfo::atomicAmount() const
{
    return m_pimpl->amount();
}

QString TransactionInfo::displayAmount() const
{
    return WalletManager::instance()->displayAmount(m_pimpl->amount());
}

QString TransactionInfo::fee() const
{
    if(m_pimpl->fee() == 0)
        return "";
    return WalletManager::instance()->displayAmount(m_pimpl->fee());
}

quint64 TransactionInfo::blockHeight() const
{
    return m_pimpl->blockHeight();
}

QSet<quint32> TransactionInfo::subaddrIndex() const
{
    QSet<quint32> result;
    for (uint32_t i : m_pimpl->subaddrIndex())
        result.insert(i);
    return result;
}

quint32 TransactionInfo::subaddrAccount() const
{
    return m_pimpl->subaddrAccount();
}

QString TransactionInfo::label() const
{
    return QString::fromStdString(m_pimpl->label());
}

quint64 TransactionInfo::confirmations() const
{
    return m_pimpl->confirmations();
}

quint64 TransactionInfo::unlockTime() const
{
    return m_pimpl->unlockTime();
}

QString TransactionInfo::hash() const
{
    return QString::fromStdString(m_pimpl->hash());
}

QDateTime TransactionInfo::timestamp() const
{
    QDateTime result = QDateTime::fromTime_t(m_pimpl->timestamp());
    return result;
}

QString TransactionInfo::date() const
{
    return timestamp().date().toString(Qt::ISODate);
}

QString TransactionInfo::time() const
{
    return timestamp().time().toString(Qt::ISODate);
}

QString TransactionInfo::paymentId() const
{
    return QString::fromStdString(m_pimpl->paymentId());
}

QString TransactionInfo::destinations_formatted() const
{
    QString destinations;
    for (auto const& t: transfers()) {
        if (!destinations.isEmpty())
          destinations += "<br> ";
        destinations +=  WalletManager::instance()->displayAmount(t->amount()) + ": " + t->address();
    }
    return destinations;
}

QList<Transfer*> TransactionInfo::transfers() const
{
    if (!m_transfers.isEmpty()) {
        return m_transfers;
    }

    for(auto const& t: m_pimpl->transfers()) {
        TransactionInfo * parent = const_cast<TransactionInfo*>(this);
        Transfer * transfer = new Transfer(t.amount, QString::fromStdString(t.address), parent);
        m_transfers.append(transfer);
    }
    return m_transfers;
}

TransactionInfo::TransactionInfo(Nejcoin::TransactionInfo *pimpl, QObject *parent)
    : QObject(parent), m_pimpl(pimpl)
{

}
