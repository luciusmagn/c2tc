#include "c2.h"
#include "inttypes.h"
#include "shared.h"

void cleanup_trg(target* trg)
{
    for (int32 i = 0; i < vector_total(trg->nodes); i++)
        cleanup(vector_get(trg->nodes, i));
    mpc_ast_print(vector_get(trg->nodes, 0));
}

void cleanup(mpc_ast_t* node)
{
    deluseless(node);
    for (int32 i = 0; i < node->children_num; i++)
        cleanup(node->children[i]);
}

void deluseless(mpc_ast_t* node)
{
    int32 found = 0;
    for (;;)
    {
        for (int32 i = 0; i < node->children_num; i++)
        {
            if (strcmp(node->children[i]->tag, "char") == 0 && strcmp(node->children[i]->contents, ";") == 0)
            {
                found++;
                printf("found:%d", found);
                node->children_num--;
                remove_element(node->children, i, node->children_num + 1);
                break;
            }
            if (strcmp(node->children[i]->tag, "char") == 0 && strcmp(node->children[i]->contents, "\"") == 0)
            {
                found++;
                printf("found:%d", found);
                node->children_num--;
                remove_element(node->children, i, node->children_num + 1);
            }
            if (strcmp(node->children[i]->tag, "char") == 0 && strcmp(node->children[i]->contents, "'") == 0 )
            {
                found++;
                printf("found:%d", found);
                node->children_num--;
                remove_element(node->children, i, node->children_num + 1);
            }
        }
        if (found == 0)
            return;
        found = 0;
    }
}
