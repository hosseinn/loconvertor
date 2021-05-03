/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*************************************************************************
 *
 *  The Contents of this file are made available subject to the terms of
 *  the BSD license.
 *
 *  Copyright 2000, 2010 Oracle and/or its affiliates.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of Sun Microsystems, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 *  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 *  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 *  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *************************************************************************/

// Simple client application using the UnoUrlResolver service.
#include <stdio.h>
#include <wchar.h>

#include <sal/main.h>
#include <cppuhelper/bootstrap.hxx>

#include <osl/file.hxx>
#include <osl/process.h>
#include <rtl/process.h>
#include <rtl/bootstrap.hxx>

#include "com/sun/star/beans/XPropertySet.hpp"
#include "com/sun/star/bridge/XUnoUrlResolver.hpp"
#include "com/sun/star/frame/Desktop.hpp"
#include "com/sun/star/frame/XComponentLoader.hpp"
#include "com/sun/star/lang/XMultiComponentFactory.hpp"
#include "com/sun/star/registry/XSimpleRegistry.hpp"
#include "com/sun/star/text/XTextDocument.hpp"
#include "com/sun/star/frame/XStorable.hpp"

#include <string.h>

using namespace css::uno;
using namespace css::lang;
using namespace css::beans;
using namespace css::bridge;
using namespace css::frame;
using namespace css::registry;
using namespace css::text;
//using namespace css::util;
using namespace rtl;

int main()
{
    OUString sLOPath(LO_ROOT);
    OUString sArgDocUrl("../loconvertor/test.odt");

    OUString sConnectionString("uno:socket,host=localhost,port=2083;urp;StarOffice.ServiceManager");

    OUString sRDB = OUString("file://")+ sLOPath + OUString("/program/types/offapi.rdb");
    rtl::Bootstrap::set(OUString::createFromAscii("URE_MORE_TYPES"),
    rtl::Bootstrap::encode(sRDB));

    OUString sAbsoluteDocUrl, sWorkingDir, sDocPathUrl;
    osl_getProcessWorkingDir(&sWorkingDir.pData);
    osl::FileBase::getFileURLFromSystemPath( sArgDocUrl, sDocPathUrl);
    osl::FileBase::getAbsoluteFileURL( sWorkingDir, sDocPathUrl, sAbsoluteDocUrl);

    Reference< XComponentContext > xComponentContext(::cppu::defaultBootstrap_InitialComponentContext());

    /* Gets the service manager instance to be used (or null). This method has
       been added for convenience, because the service manager is an often used
       object.
    */
    Reference< XMultiComponentFactory > xMultiComponentFactoryClient(
        xComponentContext->getServiceManager() );

    /* Creates an instance of a component which supports the services specified
       by the factory.
    */
    Reference< XInterface > xInterface =
        xMultiComponentFactoryClient->createInstanceWithContext(
            "com.sun.star.bridge.UnoUrlResolver",
            xComponentContext );

    Reference< XUnoUrlResolver > resolver( xInterface, UNO_QUERY );

    // Resolves the component context from the office, on the uno URL given by argv[1].
    try
    {
        xInterface = Reference< XInterface >(
            resolver->resolve( sConnectionString ), UNO_QUERY );
    }
    catch ( Exception& e )
    {
        printf("Error: cannot establish a connection using '%s':\n       %s\n",
               OUStringToOString(sConnectionString, RTL_TEXTENCODING_ASCII_US).getStr(),
               OUStringToOString(e.Message, RTL_TEXTENCODING_ASCII_US).getStr());
        exit(1);
    }

    // gets the server component context as property of the office component factory
    Reference< XPropertySet > xPropSet( xInterface, UNO_QUERY );
    xPropSet->getPropertyValue("DefaultContext") >>= xComponentContext;

    // gets the service manager from the office
    Reference< XMultiComponentFactory > xMultiComponentFactoryServer(
        xComponentContext->getServiceManager() );

    /* Creates an instance of a component which supports the services specified
       by the factory. Important: using the office component context.
    */
    Reference < XDesktop2 > xComponentLoader = Desktop::create(xComponentContext);

    /* Loads a component specified by a URL into the specified new or existing
       frame.
    */


    Sequence<PropertyValue> loadProperties(1);
    loadProperties[0] = *new PropertyValue();
    loadProperties[0].Name = OUString("Hidden");
    loadProperties[0].Value = *new Any(true);

    Reference< XComponent > xComponent = xComponentLoader->loadComponentFromURL(
        sAbsoluteDocUrl, OUString( "_blank" ), 0,
        loadProperties);

    Reference<XTextDocument> xDocument(xComponent, UNO_QUERY);
    try
    {
        Reference<XStorable> xStorable(xDocument, UNO_QUERY_THROW );

        Sequence<PropertyValue> storeProps(3);
        storeProps[0] = *new PropertyValue();
        storeProps[0].Name = OUString("FilterName");
        storeProps[0].Value = *new Any(OUString("writer_pdf_Export"));
        storeProps[1] = *new PropertyValue();
        storeProps[1].Name = OUString("Overwrite");
        storeProps[1].Value = *new Any(true);
        storeProps[2] = *new PropertyValue();
        storeProps[2].Name = OUString("SelectPdfVersion");
        storeProps[2].Value = *new Any(1);

        xStorable->storeToURL(OUString("file:///~/test.pdf"), storeProps);
    }
    catch( Exception& e) {
        printf("%s", "Can not open the file.\n");
    }

    // dispose the local service manager
    Reference< XComponent >::query( xMultiComponentFactoryClient )->dispose();

    return 0;
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
