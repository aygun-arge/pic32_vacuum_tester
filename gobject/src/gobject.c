


/*=========================================================  INCLUDE FILES  ==*/

#include "base/list.h"

#include "epa_gobject.h"
#include "gobject.h"
#include "gtheme.h"
#include "ghal.h"

#include "gbackground.h"
#include "gbutton.h"
#include "gtext.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/

struct gPage {
    esSls               sentinel;
    struct gHalPage *   halPage;
    bool                isInteractive;
};

struct gPageEntry {
    esSls               list;
    struct gObject      object;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

static const ES_MODULE_INFO_CREATE("gobject", "Graphic Object", "Nenad Radulovic");

static esEpa *          Handler;
static esMem *          Memory;
static struct gPage *   CurrentPage;

static const struct gMethod * ObjectMethods[] = {
    [GO_BACKGROUND] = &BackgroundMethods,
    [GO_BUTTON]     = &ButtonMethods,
    [GO_TEXT]       = &TextMethods
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

/*--  Page  ------------------------------------------------------------------*/

static struct gPageEntry * createPageEntry(const struct gObjectDescriptor * od) {
    esError             error;
    struct gPageEntry * pageEntry;

    ES_ENSURE(error = esMemAlloc(Memory, sizeof(*pageEntry), (void **)&pageEntry));

    if (error != ES_ERROR_NONE) {
        goto ALLOC_PAGE_OBJECT;
    }
    pageEntry->object.method                   = ObjectMethods[od->type];

    if (pageEntry->object.method->init != NULL) {
        pageEntry->object.method->init(&pageEntry->object);
    }
    pageEntry->object.id                       = od->id;
    pageEntry->object.attribute.isVisible      = true;
    pageEntry->object.attribute.eventGenerator = 0u;

    if (pageEntry->object.method->themedColor != NULL) {
        pageEntry->object.color                =
            pageEntry->object.method->themedColor(od->color);
    } else {
        pageEntry->object.color                = NULL;
    }
    pageEntry->object.geometry.position.x      = od->x;
    pageEntry->object.geometry.position.y      = od->y;

    if (pageEntry->object.method->themedWidth != NULL) {
        pageEntry->object.geometry.size.width  =
            pageEntry->object.method->themedWidth(od->size);
    } else {
        pageEntry->object.geometry.size.width  = 0u;
    }

    if (pageEntry->object.method->themedHeight != NULL) {
        pageEntry->object.geometry.size.height  =
            pageEntry->object.method->themedHeight(od->size);
    } else {
        pageEntry->object.geometry.size.height  = 0u;
    }
    pageEntry->object.geometry.alignment.hAlign = od->hAlign;
    pageEntry->object.geometry.alignment.vAlign = od->vAlign;
    pageEntry->object.property                  = NULL;

    if (od->property != NULL) {
        ES_ENSURE(error = esMemAlloc(
            Memory,
            od->propertySize,
            (void **)&pageEntry->object.property));

        if (error != ES_ERROR_NONE) {
            goto ALLOC_PROPERTY;
        }
        memcpy(pageEntry->object.property, od->property, od->propertySize);
    }

    return (pageEntry);
ALLOC_PROPERTY:
    esMemFree(Memory, pageEntry);
ALLOC_PAGE_OBJECT:

    return (NULL);
}

static void deletePageEntry(struct gPageEntry * pageEntry) {
    
    if (pageEntry->object.property != NULL) {
        esMemFree(Memory, pageEntry->object.property);
    }
    esMemFree(Memory, pageEntry);
}

static bool isPageEntryInteractive(const struct gPageEntry * pageEntry) {

    if ((pageEntry->object.attribute.isVisible) &&
        (pageEntry->object.attribute.eventGenerator != 0)) {

        return (true);
    } else {

        return (false);
    }
}

static struct gHalPage * redrawPage(struct gPage * page) {
    struct gHalPage *   halPage;
    esSls *             current;

    halPage     = gHalPageCreate();
    current     = esSlsGetHead(&page->sentinel);

    while (current != &page->sentinel) {

        struct gPageEntry * pageEntry;
        struct gObject *    object;

        pageEntry = ES_SLS_NODE_ENTRY(struct gPageEntry, list, current);
        object    = &pageEntry->object;

        if (object->attribute.isVisible) {

            if (object->attribute.eventGenerator != 0u) {
                gHalSetHotspot(
                    halPage,
                    &object->geometry,
                    object->attribute.eventGenerator);
            }
            object->method->draw(halPage, object);
        }
        current = current->next;
    }
    gHalPageFlush(halPage);

    return (halPage);
}

static struct gObject * findObject(struct gPage * page, uint16_t id) {
    esSls *             current;

    current = page->sentinel.next;

    while (current != &page->sentinel) {
        struct gPageEntry * pageEntry;

        pageEntry = ES_SLS_NODE_ENTRY(struct gPageEntry, list, current);

        if (pageEntry->object.id == id) {

            return (&pageEntry->object);
        }
        current = current->next;
    }

    return (NULL);
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void initGobjectModule(esMem * memory, esEpa * handler) {
    gHalInit();

    Memory  = memory;
    Handler = handler;

    esEpaCreate(&GobjectEpa, &GobjectSm, memory, &Gobject);
}

esEpa * gObjectModuleHandler(void) {

    return (Handler);
}

struct gPage * gPageCreate(struct gObjectDescriptor * od) {
    esError             error;
    struct gPage *      page;
    esSls *             current;

    ES_ENSURE(error = esMemAlloc(Memory, sizeof(*page), (void **)&page));

    if (error != ES_ERROR_NONE) {
        goto ALLOC_PAGE_SENTINEL;
    }
    esSlsSentinelInit(&page->sentinel);
    page->isInteractive = false;
    current = &page->sentinel;

    while (od->type != GO_PAGE_END) {
        struct gPageEntry * pageEntry;

        pageEntry = createPageEntry(od);

        if (pageEntry == NULL) {
            goto ALLOC_PAGE_ENTRY;
        }
        esSlsNodeInit(&pageEntry->list);
        esSlsNodeAddAfter(current, &pageEntry->list);

        if (isPageEntryInteractive(pageEntry)) {
            page->isInteractive = true;
        }
        current = &pageEntry->list;
        od++;
    }
    
    return (page);
ALLOC_PAGE_ENTRY:

    gPageDestroy(page);
ALLOC_PAGE_SENTINEL:

    return (NULL);
}

void gPageDestroy(struct gPage * page) {

    if (CurrentPage == page) {
        CurrentPage = NULL;
    }
    
    if (page->isInteractive) {
        esError         error;
        esEvent *       event;

        ES_ENSURE(error = esEventCreate(sizeof(esEvent), GO_TOUCH_STOP, &event));

        if (error == ES_ERROR_NONE) {
            ES_ENSURE(esEpaSendAheadEvent(Gobject, event));
        }
    }
    
    while (esSlsIsEmpty(&page->sentinel) == false) {
        esSls *         current;
        struct gPageEntry * pageEntry;

        current = esSlsGetHead(&page->sentinel);
        pageEntry = ES_SLS_NODE_ENTRY(struct gPageEntry, list, current);
        deletePageEntry(pageEntry);
        esSlsNodeRm(&page->sentinel, current);
    }
    esMemFree(Memory, page);
}

void gPageSetActive(struct gPage * page) {
    CurrentPage = page;
    CurrentPage->halPage = redrawPage(page);

    if (page->isInteractive) {
        esError         error;
        esEvent *       event;
        ES_ENSURE(error = esEventCreate(sizeof(esEvent), GO_TOUCH_START, &event));

        if (error == ES_ERROR_NONE) {
            ES_ENSURE(esEpaSendEvent(Gobject, event));
        }
    }
}

struct gHalPage * gPageGetHalPage(void) {

    if (CurrentPage != NULL) {

        return (CurrentPage->halPage);
    } else {

        return (NULL);
    }
}

void gObjectSetX(struct gPage * page, uint16_t id, uint16_t x) {
    struct gObject * object;

    object = findObject(page, id);

    if (object != NULL) {
        object->geometry.position.x = x;
    }
}

void gObjectSetY(struct gPage * page, uint16_t id, uint16_t y) {
    struct gObject * object;

    object = findObject(page, id);

    if (object != NULL) {
        object->geometry.position.y = y;
    }
}

void gObjectSetSize(struct gPage * page, uint16_t id, enum gThemeSizes size) {
    struct gObject * object;

    object = findObject(page, id);

    if (object != NULL) {
        object->geometry.size.width  = object->method->themedWidth(size);
        object->geometry.size.height = object->method->themedHeight(size);
    }
}

void gObjectSetSizeWidth(struct gPage * page, uint16_t id, uint16_t width) {
    struct gObject * object;

    object = findObject(page, id);

    if (object != NULL) {
        object->geometry.size.width = width;
    }
}

void gObjectSetSizeHeight(struct gPage * page, uint16_t id, uint16_t height) {
    struct gObject * object;

    object = findObject(page, id);

    if (object != NULL) {
        object->geometry.size.height = height;
    }
}

void gObjectSetColor(struct gPage * page, uint16_t id, enum gThemeColors color) {
    struct gObject * object;

    object = findObject(page, id);

    if (object != NULL) {
        object->color = object->method->themedColor(color);
    }
}

void gObjectAttributeSetVisible(struct gPage * page, uint16_t id, bool isVisible) {
    struct gObject * object;

    object = findObject(page, id);

    if (object != NULL) {
        object->attribute.isVisible = isVisible;
    }
}

void gObjectPropertySetEventId(struct gPage * page, uint16_t id, uint16_t eventId) {
    struct gObject * object;

    object = findObject(page, id);

    if (object != NULL) {
        object->attribute.eventGenerator = eventId;
    }
}

void gObjectPropertySetLabel(struct gPage * page, uint16_t id, const char * label) {
    struct gObject * object;

    object = findObject(page, id);

    if (object != NULL) {
        if (object->method->propertySetLabel != NULL) {
            object->method->propertySetLabel(object, label);
        }
    }
}

void gObjectPropertySetFont(struct gPage * page, uint16_t id, uint16_t font) {
    struct gObject * object;

    object = findObject(page, id);

    if (object != NULL) {
        if (object->method->propertySetFont != NULL) {
            object->method->propertySetFont(object, font);
        }
    }
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of gobject.c
 ******************************************************************************/
