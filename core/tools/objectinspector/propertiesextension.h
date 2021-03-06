/*
  propertiesextension.h

  This file is part of GammaRay, the Qt application inspection and
  manipulation tool.

  Copyright (C) 2014-2015 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Anton Kreuzkamp <anton.kreuzkamp@kdab.com>

  Licensees holding valid commercial KDAB GammaRay licenses may use this file in
  accordance with GammaRay Commercial License Agreement provided with the Software.

  Contact info@kdab.com if any conditions of this licensing are not clear to you.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GAMMARAY_OBJECTINSPECTOR_PROPERTIESEXTENSION_H
#define GAMMARAY_OBJECTINSPECTOR_PROPERTIESEXTENSION_H

#include "common/tools/objectinspector/propertiesextensioninterface.h"
#include "core/propertycontrollerextension.h"

#include <QPointer>

namespace GammaRay {

class PropertyController;

class ObjectDynamicPropertyModel;
class ObjectStaticPropertyModel;
class MetaPropertyModel;
class AggregatedPropertyModel;

class PropertiesExtension : public PropertiesExtensionInterface, public PropertyControllerExtension
{
  Q_OBJECT
  Q_INTERFACES(GammaRay::PropertiesExtensionInterface)

  public:
    explicit PropertiesExtension(PropertyController *controller);
    ~PropertiesExtension();

    void navigateToValue(int modelRow) Q_DECL_OVERRIDE;
    void setProperty(const QString &name, const QVariant &value) Q_DECL_OVERRIDE;
    void resetProperty(const QString &name) Q_DECL_OVERRIDE;

    bool setObject(void *object, const QString &typeName) Q_DECL_OVERRIDE;
    bool setQObject(QObject *object) Q_DECL_OVERRIDE;
    bool setMetaObject(const QMetaObject* metaObject) Q_DECL_OVERRIDE;

  private:
    ObjectStaticPropertyModel *m_staticPropertyModel;
    ObjectDynamicPropertyModel *m_dynamicPropertyModel;
    MetaPropertyModel *m_metaPropertyModel;

    AggregatedPropertyModel *m_aggregatedPropertyModel;
    QPointer<QObject> m_object;
};

}

#endif // PROPERTIESEXTENSION_H
