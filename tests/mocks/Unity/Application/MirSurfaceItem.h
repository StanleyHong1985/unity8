/*
 * Copyright (C) 2014 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MIRSURFACEITEM_H
#define MIRSURFACEITEM_H

#include "MirSurfaceItemModel.h"

#include <QQuickItem>
#include <QUrl>

class Session;

class MirSurfaceItem : public QQuickItem
{
    Q_OBJECT
    Q_ENUMS(Type)
    Q_ENUMS(State)

    Q_PROPERTY(Type type READ type NOTIFY typeChanged)
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(MirSurfaceItem* parentSurface READ parentSurface NOTIFY parentSurfaceChanged DESIGNABLE false)
    Q_PROPERTY(MirSurfaceItemModel* childSurfaces READ childSurfaces DESIGNABLE false CONSTANT)

public:
    enum Type {
        Normal,
        Utility,
        Dialog,
        Overlay,
        Freestyle,
        Popover,
        InputMethod,
    };

    enum State {
        Unknown,
        Restored,
        Minimized,
        Maximized,
        VertMaximized,
        /* SemiMaximized, */
        Fullscreen,
    };

    ~MirSurfaceItem();

    //getters
    Session* session() const { return m_session; }
    Type type() const { return m_type; }
    State state() const { return m_state; }
    QString name() const { return m_name; }
    MirSurfaceItem* parentSurface() const { return m_parentSurface; }

    void setSession(Session* item);
    void setScreenshot(const QUrl& screenshot);

    Q_INVOKABLE void setState(State newState);
    Q_INVOKABLE void release();

    Q_INVOKABLE void addChildSurface(MirSurfaceItem* surface);
    void insertChildSurface(uint index, MirSurfaceItem* surface);
    void removeChildSurface(MirSurfaceItem* surface);

Q_SIGNALS:
    void typeChanged(Type);
    void stateChanged(State);
    void parentSurfaceChanged(MirSurfaceItem*);

    void removed();

    void inputMethodRequested();
    void inputMethodDismissed();

    void aboutToBeDestroyed();

private Q_SLOTS:
    void onFocusChanged();
    void onComponentStatusChanged(QQmlComponent::Status status);
    void onQmlWantInputMethodChanged();

private:
    explicit MirSurfaceItem(const QString& name,
                            Type type,
                            State state,
                            const QUrl& screenshot,
                            const QString &qmlFilePath = QString(),
                            QQuickItem *parent = 0);

    MirSurfaceItemModel* childSurfaces() const;
    void setParentSurface(MirSurfaceItem* surface);

    void createQmlContentItem();
    void printComponentErrors();

    Session* m_session;
    const QString m_name;
    const Type m_type;
    State m_state;

    MirSurfaceItem* m_parentSurface;
    MirSurfaceItemModel* m_children;

    QQmlComponent *m_qmlContentComponent;
    QQuickItem *m_qmlItem;
    QUrl m_screenshotUrl;

    friend class SurfaceManager;
};

Q_DECLARE_METATYPE(MirSurfaceItem*)
Q_DECLARE_METATYPE(QList<MirSurfaceItem*>)

#endif // MIRSURFACEITEM_H
