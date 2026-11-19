#include <stdio.h>
#include <stdlib.h>
#include <libxml/parser.h>
#include <libxml/xmlschemas.h>

enum {
    ERROR_OCCURED = -1, // Error in validation
    NOT_VALID = 0,      // Document is not valid
    VALID = 1           // Document is valid
};

/**
 * Fonction de validation d'un arbre DOM à l'aide d'un XML Schema
 **/
int validation_schema(xmlDocPtr doc, const char *xml_schema, int afficher_erreurs) {
    int ret;
    xmlSchemaPtr schema;
    xmlSchemaValidCtxtPtr vctxt;
    xmlSchemaParserCtxtPtr pctxt;

    // Ouverture du fichier XML Schema
    fprintf(stderr, "Creating schema parser\n");
    if ((pctxt = xmlSchemaNewParserCtxt(xml_schema)) == NULL) {
        return ERROR_OCCURED;
    }
    fprintf(stderr, "Parsing schema\n");
    schema = xmlSchemaParse(pctxt);
    xmlSchemaFreeParserCtxt(pctxt);
    if (schema == NULL) {
        return ERROR_OCCURED;
    }
    fprintf(stderr, "Creating validation context\n");
    if ((vctxt = xmlSchemaNewValidCtxt(schema)) == NULL) {
        xmlSchemaFree(schema);
        return ERROR_OCCURED;
    }
    fprintf(stderr, "Setting validation error handlers\n");
    xmlSchemaSetValidErrors(vctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);

    // Validation
    fprintf(stderr, "Performing validation\n");
    ret = (xmlSchemaValidateDoc(vctxt, doc) == 0 ? VALID : NOT_VALID);
    // Libération de la mémoire
    xmlSchemaFree(schema);
    xmlSchemaFreeValidCtxt(vctxt);

    return ret;
}

void usage() {
    printf("validation_schema [fichier XML à valider] [fichier XML Schema]\n");
}

int main(int argc, char **argv) {
    xmlDocPtr doc;

    if (argc != 3) {
        usage();
        return EXIT_FAILURE;
    }
    xmlKeepBlanksDefault(0); // Ignore les noeuds texte composant la mise en forme
    // Ouverture du fichier XML et transformation de celui-ci en un arbre DOM
    if ((doc = xmlParseFile(argv[2])) == NULL) {
        printf("XML document invalid\n");
        return EXIT_FAILURE;
    }
    // Validation
    if (validation_schema(doc, argv[1], 1) == VALID) {
        printf("Document validation passed\n");
    } else {
        printf("Document validation failed\n");
    }
    // Libération de la mémoire
    xmlFreeDoc(doc);

    return EXIT_SUCCESS;
}
