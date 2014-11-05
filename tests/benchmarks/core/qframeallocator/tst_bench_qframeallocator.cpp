/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QObject>
#include <QtTest/QtTest>

#include <Qt3DCore/QFrameAllocator>

using namespace Qt3D;

class tst_QFrameAllocator : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void benchmarkAllocateSmall();
    void benchmarkAllocateBig();
    void benchmarkDeallocateSmall();
    void benchmarkDeallocateBig();
    void benchmarkAllocateRaw();
    void benchmarkDeallocateRaw();
};

namespace {
template<uint Size>
struct Object {
    char data[Size];
};

template<uint Size>
void benchmarkAlloc()
{
    QFrameAllocator allocator(128, 16, 128);
    QVector<Object<Size>*> items(10000);
    QBENCHMARK_ONCE {
        for (int i = 0; i < items.size(); ++i) {
            items[i] = allocator.allocate< Object<Size> >();
        }
    }
    foreach (Object<Size>* item, items) {
        allocator.deallocate(item);
    }
}

template<uint Size>
void benchmarkDealloc()
{
    QFrameAllocator allocator(128, 16, 128);
    QVector<Object<Size>*> items(10000);
    for (int i = 0; i < items.size(); ++i) {
        items[i] = allocator.allocate< Object<Size> >();
    }
    QBENCHMARK_ONCE {
        foreach (Object<Size>* item, items) {
            allocator.deallocate(item);
        }
    }
}
}

void tst_QFrameAllocator::benchmarkAllocateSmall()
{
    benchmarkAlloc<16>();
}

void tst_QFrameAllocator::benchmarkAllocateBig()
{
    benchmarkAlloc<128>();
}

void tst_QFrameAllocator::benchmarkDeallocateSmall()
{
    benchmarkDealloc<16>();
}

void tst_QFrameAllocator::benchmarkDeallocateBig()
{
    benchmarkDealloc<128>();
}

void tst_QFrameAllocator::benchmarkAllocateRaw()
{
    QFrameAllocator allocator(128, 16, 128);
    QVector<void*> items(10000);
    QBENCHMARK_ONCE {
        for (int i = 0; i < items.size(); ++i) {
            items[i] = allocator.allocateRawMemory(i % 128 + 1);
        }
    }
    for (int i = 0; i < items.size(); ++i) {
        allocator.deallocateRawMemory(items[i], i % 128 + 1);
    }
}

void tst_QFrameAllocator::benchmarkDeallocateRaw()
{
    QFrameAllocator allocator(128, 16, 128);
    QVector<void*> items(10000);
    for (int i = 0; i < items.size(); ++i) {
        items[i] = allocator.allocateRawMemory(i % 128 + 1);
    }
    QBENCHMARK_ONCE {
        for (int i = 0; i < items.size(); ++i) {
            allocator.deallocateRawMemory(items[i], i % 128 + 1);
        }
    }
}

QTEST_APPLESS_MAIN(tst_QFrameAllocator)
#include "tst_bench_qframeallocator.moc"
