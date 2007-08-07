/*
** @file containers.c
** 
** @brief Implement generic routines for containers
**
** Started on  Sat Jun  2 15:20:18 2005 jfv
** $Id: containers.c,v 1.1 2007-08-07 07:13:27 may Exp $
**
*/
#include "libaspect.h"


/**
 * @brief Create container lists
 */
int		container_linklists_create(container_t *container,
					   u_int	linktype)
{
  aspectype_t  *type;
  char		bufname[BUFSIZ];
  char		*prefix;

  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);

  /* Check for prefix (XXX: change to lookup user-configured prefixes ?) */
  switch (container->type)
    {
    case ASPECT_TYPE_BLOC:
      prefix = "bloc";
      break;
    case ASPECT_TYPE_FUNC:
      prefix = "func";
      break;
    default:
      type = aspect_type_get_by_id(container->type);
      if (!type)
	PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__,
		     "Unable to find type of container", -1);
      prefix = type->name;
    }
  
  /* Now really allocate the list */
  switch (linktype)
    {
    case CONTAINER_LINK_IN:
      snprintf(bufname, BUFSIZ, "%s_%08X_%s", prefix, *(u_long *) container->data, "inputs");
      XALLOC(__FILE__, __FUNCTION__, __LINE__, container->inlinks, sizeof(list_t), -1);
      list_init(container->inlinks, strdup(bufname), container->type);
      break;
    case CONTAINER_LINK_OUT:
      snprintf(bufname, BUFSIZ, "%s_%08X_%s", prefix, *(u_long *) container->data, "outputs");
      XALLOC(__FILE__, __FUNCTION__, __LINE__, container->outlinks, sizeof(list_t), -1);
      list_init(container->outlinks, strdup(bufname), container->type);
      break;
    default:
      PROFILER_ERR(__FILE__, __FUNCTION__, __LINE__, 
		   "Unknown link type", -1);  
    }

  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, 0);
}


/**
 * @brief Create a new container
 */
container_t	*container_create(u_int type, void *data, list_t *inlist, list_t *outlist)
{
  container_t	*newcntnr;

  PROFILER_IN(__FILE__, __FUNCTION__, __LINE__);
  XALLOC(__FILE__, __FUNCTION__, __LINE__, 
	 newcntnr, sizeof(container_t), NULL);
  newcntnr->data = data;
  newcntnr->type = type;
  if (inlist)
    newcntnr->inlinks = list_copy(inlist);
  else
    container_linklists_create(newcntnr, CONTAINER_LINK_IN);
  if (outlist)
    newcntnr->outlinks = list_copy(outlist);
  else
    container_linklists_create(newcntnr, CONTAINER_LINK_OUT);
  PROFILER_ROUT(__FILE__, __FUNCTION__, __LINE__, newcntnr);
}
