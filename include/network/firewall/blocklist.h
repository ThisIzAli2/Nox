/**
 * @author Ali Mirmohammad
 * @file blocklist.h.h
 ** This file is part of Novanix.

**Novanix is free software: you can redistribute it and/or modify
**it under the terms of the GNU Affero General Public License as published by
**the Free Software Foundation, either version 3 of the License, or
**(at your option) any later version.

**Novanix is distributed in the hope that it will be useful,
**but WITHOUT ANY WARRANTY; without even the implied warranty of
**MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
**GNU Affero General Public License for more details.

**You should have received a copy of the GNU Affero General Public License
**along with Novanix. If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef __NOVANIX_KERNEL_BLOCK_LIST_H
#define __NOVANIX_KERNEL_BLOCK_LIST_H

#include <common/init.hpp>

#define MAX_FIREWALL_IPS 100

typedef unsigned int ip_t;



typedef struct Node{
    ip_t ip;
    struct Node *next;
} Node; 

static Node* firewall_list = 0;


static Node node_pool[MAX_FIREWALL_IPS];
static INTEGER node_used[MAX_FIREWALL_IPS]; // 0=free,1=used


Node* allocate_node();


VOID free_node(Node* node);
INTEGER firewall_append(ip_t ip);

INTEGER firewall_remove(ip_t ip);
INTEGER firewall_check(ip_t ip);

INTEGER search_for_ip(ip_t ip);


#endif /*__NOVANIX_KERNEL_BLOCK_LIST_H*/