/* Copyright 2014, ACSE & CADIEEL
 *    ACSE   : http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *    CADIEEL: http://www.cadieel.org.ar
 * All rights reserved.
 *
 * This file is part of CIAA Firmware.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/** \brief Short description of this file
 **
 ** TODO Long description of this file
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Template Template to start a new module FIXME
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MZ		Marcos Ziegler
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20160101 v0.0.1 MZ initial version
 */

/*
TODO:
-Utilizar solo memoria estatica, salvo excepciones que lo justifiquen
-Agregar mutex o semaforos o colas segun corresponda para soportar multitasking
-Implementar un file system para el root, para asi no necesitar de un device real para crear archivos
*/

/*==================[inclusions]=============================================*/
#include "ciaaPOSIX_stdlib.h"
#include "ciaaBlockDevices.h"
#include "ciaaPOSIX_stdio.h"
#include "ciaaPOSIX_string.h"
#include "ciaaPOSIX_stdbool.h"
#include "vfs.h"

/*==================[macros and definitions]=================================*/

#define ASSERT_MSG(cond, msg) assert_msg((cond), (msg), __FILE__, __LINE__)

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/** \brief Create a node and link it as its parents child
 **
 ** This function may be used in other modules. Must be extern
 **
 ** \param[in] parent directory in which to create the node
 ** \param[in] name name of the new child
 ** \param[in] namelen length of name string
 ** \param[in] mode attributes of the new file
 ** \return created node if success, else NULL
 **/
extern vnode_t *vfs_create_child(vnode_t *parent, const char *name, size_t namelen, mode_t mode);

/** \brief 
 **
 ** 
 **
 ** \param[in] node
 ** \param[in] path
 ** \param[in] len_p
 ** \return -1 if an error occurs, in other case 0
 **/
static int vfs_get_mntpt_path_rec(vnode_t *node, char *path, uint16_t *len_p);

/** \brief get the path relative to the mountpoint
 **
 ** 
 **
 ** \param[in] node node whose mount point path is to be found
 ** \param[out] path string which will contain the mountpoint path
 ** \param[out] len_p size of path string
 ** \return -1 if an error occurs, in other case 0
 **/
static int vfs_get_mntpt_path(vnode_t *node, char *path, uint16_t *len_p);

/** \brief alloc memory for new node and initialize contents
 **
 ** 
 **
 ** \param[in] name
 ** \param[in] name_len
 ** \return created node if success, else NULL
 **/
static vnode_t *vfs_node_alloc(const char *name, size_t name_len);

/** \brief free a node
 **
 ** preconditions:
 **    Supongo que ya estan liberados de memoria los elementos internos de node
 **
 ** \param[in] node which will be deleted
 **/
static void vfs_node_free(vnode_t *node);

/** \brief create subtree given a path
 **  
 ** Devuelve -1 si el directorio ya existia
 ** Crea directorios intermedios
 ** TODO: Si recibe un path sin el '/' inicial, es un relative path. Crea el arbol a partir del nodo entregado
 ** Si esta el '/' inicial, crea el arbol a partir del root,  como ahora
 ** La hoja creada es de tipe VFS_FTREG
 ** 
 ** \param[in] path
 ** \param[out] inode_p
 ** \return -1 if the directory already exists FIXME
 **/
static int vfs_inode_reserve(const char *path, vnode_t **inode_p);

/** \brief search inode by path
 **
 ** postconditions:
 **    path_p points to the beginning first non-found element in path
 **    ret_node_p points to the last found node in the path
 **
 ** \param[in] path_p pointer to the path string to be searched
 ** \param[in] ret_node_p pointer to the found node address
 ** \return -1 if an error occurs, in other case 0
 **/
static int vfs_inode_search(char **path_p, vnode_t **ret_node_p);

/** \brief get a driver of a specified filesystem by name
 **
 ** 
 **
 ** \param[in] driver_name name of the driver as given in its corresponding filesystem_driver struct
 ** \return pointer to searched fs driver if success, NULL otherwise
 **/
static struct filesystem_driver *vfs_get_driver(const char *driver_name);

/** \brief initialize file descriptor table
 **
 ** alloc memory, initialize values, etc.
 ** 
 ** \return -1 if an error occurs, in other case 0
 **/
static int file_descriptor_table_init(void);

/** \brief 
 **
 ** 
 **
 ** \param[in] node the new file descriptor will be linked to this file
 ** \return pointer to created descriptor structure if success, else NULL
 **/
static struct file_desc *file_desc_create(vnode_t *node);

/** \brief free a file descriptor
 **
 ** 
 **
 ** \param[in] file_desc descriptor to be freed
 ** \return -1 if an error occurs, in other case 0
 **/
static int file_desc_destroy(struct file_desc *file_desc);

/** \brief get a descriptor structure by its number
 **
 ** 
 **
 ** \param[in] index requested descriptor structure index
 ** \return -1 if an error occurs, in other case 0
 **/
static struct file_desc *file_desc_get(uint16_t index);

/*==================[internal data definition]===============================*/

/** \brief Nodo root
 *
 * Es el nodo padre de todo el vfs.
 * 
 */
static vnode_t *vfs_root_inode = NULL;

/** \brief Filesystem drivers declaration
 *
 * Declaro los drivers que va a utilizar el vfs. Estan definidos en sus respectivos archivos
 * 
 */
extern struct filesystem_driver ext2_driver;
extern struct filesystem_driver pseudofs_driver;
extern struct filesystem_driver blockdev_driver;

/** \brief Filesystem drivers table
 *
 * Tabla utilizada por vfs_get_driver() para buscar el driver solicitado
 * 
 */
static struct filesystem_driver *vfs_fsdriver_table[] =
{
   &ext2_driver,
   &blockdev_driver,
   NULL,
   NULL,
   NULL
};

/** \brief Filesystem drivers table
 *
 * Tabla de descriptores de archivo. 
 * 
 */
static struct file_descriptor_table _f_tab;
static struct file_descriptor_table *file_desc_tab_p = &_f_tab;


/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

static void assert_msg(int cond, char* msg, char * file, int line)
{
   if (cond)
   {
      /* assertion is ok */
      ciaaPOSIX_printf("OK: Assert in %s:%d\n", file, line);
   }
   else
   {
      ciaaPOSIX_printf("ERROR: Assert Failed in %s:%d %s\n", file, line, msg);
   }
}

static int vfs_get_mntpt_path_rec(vnode_t *node, char *path, uint16_t *len_p)
{
   uint16_t name_len;
   int ret;
   if(node->n_info.is_mount_dir==false)
   ret = vfs_get_mntpt_path_rec(node->parent_node, path, len_p);
   if(ret)
   {
      return -1;
   }
   name_len = ciaaPOSIX_strlen(node->f_info.file_name);
   if(*len_p + name_len > FS_PATH_MAX-1)
      return -1;
   ciaaPOSIX_strcpy((char *)path+(*len_p), node->f_info.file_name);
   *len_p += ciaaPOSIX_strlen(node->f_info.file_name);
   return 0;
}

static int vfs_get_mntpt_path(vnode_t *node, char *path, uint16_t *len_p)
{
   int ret;
   *len_p = 0;
   ret = vfs_get_mntpt_path_rec(node, path, len_p);
   return ret;
}

static vnode_t *vfs_node_alloc(const char *name, size_t name_len)
{
   vnode_t * new_node;

   ASSERT_MSG(name_len, "\tvfs_node_alloc(): !name_len failed");
   if (!name_len)
   {
      return NULL;
   }
   ASSERT_MSG(*name && name_len <= FS_NAME_MAX, "\tvfs_node_alloc(): !(*name) || name_len > FS_NAME_MAX failed");
   if (!(*name) || name_len > FS_NAME_MAX)
   {
      return NULL;
   }
   new_node = (vnode_t *) ciaaPOSIX_malloc(sizeof(vnode_t));
   ASSERT_MSG(new_node!=NULL, "\tvfs_node_alloc(): !(*name) || name_len > FS_NAME_MAX failed");
   if (new_node == NULL)
   {
      return NULL;
   }
   ciaaPOSIX_memset(new_node, 0, sizeof(vnode_t));
   ciaaPOSIX_strncpy(new_node->f_info.file_name, name, name_len);
   new_node->f_info.file_name[name_len] = '\0';
   new_node->f_info.file_namlen = name_len;

   return new_node;
}

/*Supongo que ya estan liberados de memoria los elementos internos de vnode_t*/
static void vfs_node_free(vnode_t *node)
{
   /* Iterate over all the siblings until node found. Then link previous node to next node */
   if(node!=NULL)
      ciaaPOSIX_free(node);
}

//Devuelve -1 si el directorio ya existia
//Crea directorios intermedios
//TODO: Si recibe un path sin el '/' inicial, es un relative path. Crea el arbol a partir del nodo entregado
//Si esta el '/' inicial, crea el arbol a partir del root,  como ahora
//La hoja creada es de tipe VFS_FTREG
static int vfs_inode_reserve(const char *path, vnode_t **inode_p)
{
   int ret;
   vnode_t *inode, *child;
   char *p_start, *p_end, *aux_path;

   aux_path = (char *)path;
   ret=vfs_inode_search(&aux_path, &inode);
   ASSERT_MSG(ret, "\tvfs_inode_reserve(): Node already exists");
   if(!ret)
   {
      //El nodo ya existe
      return -1;
   }
// *path_p va a apuntar al primer caracter del primer elemento del path que no se encuentra en el arbol
// *ret_node_p va a apuntar al ultimo nodo valido del arbol incluido en el path.
//ciaaPOSIX_printf("vfs_inode_reserve():Primer elemento que no se encuentra: %s\n", aux_path);
//ciaaPOSIX_printf("vfs_inode_reserve():Ultimo nodo valido: %s\n", inode->f_info.file_name);
   //Ahora inode apunta al ultimo nodo valido, por lo que hay que empezar creando inode->child
   //Ahora path apunta al primer caracter del primer elemento del path a crear, por lo que
   //inode->child debe ser nombrado a partir de este elemento.
   p_start=p_end=aux_path;
   for(p_start=aux_path; *p_start=='/'; p_start++);   //Salteo los '/' iniciales
   while(1)
   {
      for(p_end=p_start; *p_end!='\0' && *p_end!='/'; p_end++);
      //Ahora p_start y p_end apuntan al siguiente elemento del path
      //Node tendria que apuntar al nodo recien creado
      //ciaaPOSIX_printf("vfs_inode_reserve(): %s, %d\n", p_start, (int)(p_end-p_start));
      if(p_end==p_start)
      {
         
         return -1;
      }
      child = vfs_create_child(inode, p_start, (uint32_t)(p_end-p_start), 0);
      ciaaPOSIX_printf("vfs_inode_reserve(): Created child with name:%*s and length:%u\n",child->f_info.file_namlen, child->f_info.file_name, child->f_info.file_namlen);
      *inode_p = child;
      ASSERT_MSG(child!=NULL, "\tvfs_inode_reserve(): vfs_create_child() failed");
      if(child == NULL)
      {
         return -1;
      }
      inode=child;
      for(p_start=p_end;*p_start=='/';p_start++);
      /* Cuando termina estoy en la primera letra despues del '/' */
      if(*p_start=='\0')//Llegue al fin del path que busco
      {
         /* El ultimo node es el que corresponde */
         return 0;
      }
      /* Queda camino por seguir. El nodo creado es un directorio */
      child->n_info.type = VFS_FTDIR;
   }
}

static int vfs_inode_search(char **path_p, vnode_t **ret_node_p)
{
   uint16_t pnamelen; /* Path element namelength */
   char *p_start, *p_end;
   uint32_t ret;
   vnode_t *node;

    if (*path_p==NULL || (*path_p)[0]=='\0' || *(path_p)[0]!='/')
   {
      *ret_node_p = NULL;
      return -1;
   }

   *ret_node_p = node = vfs_root_inode;

   p_start = p_end = *path_p;
   for(;*p_start=='/';p_start++);   /* Cuando termina estoy en la primera letra despues del '/' */
   *path_p = p_start;
   if(*p_start=='\0')   /* En el path solo habian '/'. Devuelvo el root */
   {
      *ret_node_p=vfs_root_inode;
      return 0;
   }
   for(p_end=p_start; *p_end!='\0' && *p_end!='/'; p_end++);
   node = node->child_node;   /* Comienzo en el primer nivel debajo del root */
   while(node!=NULL)   /* Si el nodo es NULL significa que ninguno de los hermanos del nivel coincide con el
                        * elemento del path, entonces el path no es valido
                        */
   {
      /* Los caracteres que conforman el elemento del path van a estar comprendidos entre p_start y p_end.
       * p_start apunta a la primera letra del elemento. p_end apunta al siguiente caracter de la ultima letra del elemento.
       */
      pnamelen = p_end-p_start;   /* Longitud del nombre entre '/' sin contar el '\0' */
      while(node!=NULL)
      {
         ret = ciaaPOSIX_strncmp(p_start, node->f_info.file_name, pnamelen);
         if(!ret)
         {
            ciaaPOSIX_printf("\tCoincidieron %s con %s\n",p_start,node->f_info.file_name);
            /* Coinciden los nombres, tengo que analizar el hijo */
            break;
         }
      /* El nombre del nodo no coincide con el elemento del path. Tengo que comparar el mismo elemento del path
       * con el siguiente nodo del mismo nivel.
       * Tener que comparar el mismo elemento del path significa que p_start y p_end no deben cambiar
       */
         node = node->sibling_node;
      }
      if(node==NULL)
      {
         /* Al iterar sobre los hermanos no se encontro ningun elemento del path que coincida con el
          * nombre de alguno de los nodos de ese nivel del arbol, asi que el path es invalido.
          * *ret_node_p va a apuntar al ultimo nodo valido del path
          */
         return -1;
      }
      /* El elemento del path coincide con el nombre del nodo. Hay que seguir con el siguiente elemento
       * del path en el siguiente nivel del arbol de nodos
       */
      *ret_node_p = node;
      node = node->child_node;
      p_start = p_end;
      for(;*p_start=='/';p_start++);   //Cuando termina estoy en la primera letra despues del '/'
      *path_p = p_start;
      if(*p_start=='\0')            //Llegue al fin del path que busco
      {
         /* El ultimo node es el que corresponde */
         return 0;
      }
      for(p_end=p_start; *p_end!='\0' && *p_end!='/'; p_end++);
      /* Cuando termina p_end esta en el '/' que sigue O p_end esta en el '\0' si era ultimo elemento del path */
   }
   /* El path contiene mas elementos que la profundidad del arbol, no existe el nodo buscado. Devuelvo fallido */
   return -1;
}

static struct filesystem_driver *vfs_get_driver(const char *driver_name)
{
   int i;

   for(i=0; vfs_fsdriver_table[i]!=NULL; i++)
   {
      if(!ciaaPOSIX_strcmp(driver_name, vfs_fsdriver_table[i]->driver_name))
         return vfs_fsdriver_table[i];
   }
   return NULL;
}

/*#######################FILE_DESC######################*/

static int file_descriptor_table_init(void)
{
   ciaaPOSIX_memset(file_desc_tab_p, 0, sizeof(struct file_descriptor_table));
   return 0;
}

static struct file_desc *file_desc_create(vnode_t *node)
{
   struct file_desc *file;
   uint16_t i;

   if(file_desc_tab_p->n_busy_desc >= FILE_DESC_MAX)
      return NULL;
   
   for(i=0; i<FILE_DESC_MAX; i++)
   {
      if(file_desc_tab_p->table[i] == NULL)
         break;
   }
   ciaaPOSIX_printf("file_desc_create(): Indice obtenido: %d\n", i);
   /* Ahora i es el indice del primer file_desc desocupado en la tabla */
   if(i==FILE_DESC_MAX)   /* No hay file_desc libres */
      return NULL;
   /* Allocate new descriptor */
   file = (struct file_desc *) ciaaPOSIX_malloc(sizeof(struct file_desc));
   if(file == NULL)
      return NULL;
   ciaaPOSIX_memset(file, 0, sizeof(struct file_desc));
   file->node = node;

   file_desc_tab_p->table[i] = file;
   file_desc_tab_p->n_busy_desc++;

   
   file->node = node;
   file->index = i;

   return file;
}

static int file_desc_destroy(struct file_desc *file_desc)
{
   uint16_t index;

   index = file_desc->index;
   ciaaPOSIX_free(file_desc_tab_p->table[index]);
   file_desc_tab_p->table[index] = NULL;
   if(file_desc_tab_p->n_busy_desc)
      file_desc_tab_p->n_busy_desc--;
   return 0;
}

struct file_desc *file_desc_get(uint16_t index)
{

   if(index >= FILE_DESC_MAX)
      return NULL;

   return file_desc_tab_p->table[index];
}



/*########################/FILE_DESC#####################*/

/*########################AUXILIAR#####################*/
static int vfs_print_tree_rec(vnode_t *node, int level)
{
   int ret, i;

   if(node==NULL)
   {
      return 0;
   }
   for(i=0; i<level; i++)
      ciaaPOSIX_printf("---");
   ciaaPOSIX_printf("%s\n",node->f_info.file_name);

   ret=vfs_print_tree_rec(node->child_node, level+1);
   if(ret)
   {
      return -1;
   }
   ret=vfs_print_tree_rec(node->sibling_node, level);
   if(ret)
   {
      return -1;
   }

   return 0;
}

int vfs_print_tree(void)
{
   return vfs_print_tree_rec(vfs_root_inode, 0);
}
/*########################/AUXILIAR#####################*/

/*==================[external functions definition]==========================*/

extern vnode_t *vfs_create_child(vnode_t *parent, const char *name, size_t namelen, mode_t mode)
{
   vnode_t *child;

   child = vfs_node_alloc(name, namelen);
   ASSERT_MSG(child!=NULL, "\tvfs_create_child(): vfs_node_alloc() failed");
   if(child == NULL)
      return NULL;
   child->sibling_node = parent->child_node;
   parent->child_node = child;
   child->parent_node = parent;
   /* Copio la informacion de filesystem. No hago copia nueva, solo apunto a lo del padre */
   ciaaPOSIX_memcpy(&(child->fs_info), &(parent->fs_info), sizeof(struct filesystem_info));
   child->f_info.file_name[namelen] = '\0';
   child->f_info.file_pointer=0;
   child->f_info.down_layer_info=NULL;
   return child;
   
}

extern int ciaaFS_init(void)
{
   vnode_t *aux_inode;
   int ret;
   ciaaDevices_deviceType * device;

   vfs_root_inode = (vnode_t *) ciaaPOSIX_malloc(sizeof(vnode_t));
   if(vfs_root_inode==NULL)
   {
      return -1;
   }
   vfs_root_inode->n_info.is_mount_dir = true;
   vfs_root_inode->n_info.type = VFS_FTDIR;
   vfs_root_inode->f_info.file_name[0] = '\0';
   ret = file_descriptor_table_init();
   ASSERT_MSG(-1 != ret, "vfs_init(): file_descriptor_table_init() failed");
   if(ret)
   {
      return -1;
   }
   /* Crear dispositivos */
   ret = vfs_inode_reserve("/dev/block/fd/0", &aux_inode);
   ASSERT_MSG(-1 != ret, "vfs_init(): vfs_inode_reserve() failed");
   if(aux_inode == NULL)
   {
      /* No se pudo crear el nodo */
      return -1;
   }

   aux_inode->n_info.type = VFS_FTBLK;
   aux_inode->fs_info.device = ciaaDevices_getDevice("/dev/block/fd/0");
   ASSERT_MSG(aux_inode->fs_info.device!=NULL, "vfs_init(): vfs_get_driver() failed");
   if(aux_inode->fs_info.device == NULL)
   {
      return -1;
   }
   device = ciaaBlockDevices_open("/dev/block/fd/0", (ciaaDevices_deviceType *)aux_inode->fs_info.device, ciaaPOSIX_O_RDWR);
   ASSERT_MSG(device!=NULL, "vfs_init(): failed to open device");

   aux_inode->fs_info.drv = vfs_get_driver("BLOCKDEV");
   ASSERT_MSG(aux_inode->fs_info.drv != NULL, "vfs_init(): vfs_get_driver() failed");
   if(aux_inode->fs_info.drv == NULL)
   {
      return -1;
   }

   ret = vfs_inode_reserve("/dev/char/fd/0", &aux_inode);
   ASSERT_MSG(-1 != ret, "vfs_init(): vfs_inode_reserve() failed");
   if(aux_inode == NULL)
   {
      /* No se pudo crear el nodo */
      return -1;
   }

   return 0;
}

/** \brief Format a device
 *
 * 
 * 
 */
extern int ciaaFS_format(const char *device_path, const char *fs_type)
{
   vnode_t *devnode;
   char *devpath;
   struct filesystem_driver *fs_driver;
   int ret;

   devpath = (char *)device_path;
   ret = vfs_inode_search(&devpath, &devnode);
   ASSERT_MSG(ret==0, "mount(): format() failed. Device doesnt exist");
   if(ret)
   {
      /* El nodo del device no existe */
      return -1;
   }

   fs_driver = vfs_get_driver(fs_type);
   ASSERT_MSG(fs_driver!=NULL, "format(): Device node not valid");
   if(fs_driver == NULL)
   {
      /* No existe un driver con el nombre dado */
      return -1;
   }

   ASSERT_MSG(devnode->n_info.type == VFS_FTBLK, "format(): Target file not a device");
   if(devnode->n_info.type != VFS_FTBLK)
   {
      /* El supuesto nodo device no es device */
      return -1;
   }

   /* Llamo a la funcion de bajo nivel */
   ret = fs_driver->driver_op->fs_format(devnode);
   ASSERT_MSG(ret==0, "format(): Lower layer format failed");
   if(ret)
   {
      /* Fallo el format de bajo nivel */
      return -1;
   }
   return 0;
}

/** \brief VFS mount
 *
 * Ir a la definicion de POSIX
 * 
 */
extern int ciaaFS_mount(char *device_path,  char *target_path, char *fs_type)
{
   char *devpath, *tpath;
   struct filesystem_driver *fs_driver;
   int ret;
   vnode_t *targetnode, *devnode;

   /* TODO: Agregar el tema de crear un nuevo nodo donde agregar el mount, validar que no existe */
   /* En Nuttx hace ret = inode_reserve(target, &mountpt_inode); target es el path del dir destino */
   /* Cancela si ya existe un node en el path
    */
   tpath = target_path;
   ret = vfs_inode_reserve(tpath, &targetnode);
   ASSERT_MSG(ret==0, "mount(): vfs_inode_reserve() failed creating target node");
   if(ret)
   {
      /* El directorio ya existe */
      return -1;
   }
   devpath = device_path;
   ret = vfs_inode_search(&devpath, &devnode);
   ASSERT_MSG(ret==0, "mount(): vfs_inode_search() failed. Device doesnt exist");
   if(ret)
   {
      /* El nodo del device no existe */
      return -1;
   }
   ASSERT_MSG(devnode->n_info.type == VFS_FTBLK, "mount(): Device node not valid");
   if(devnode->n_info.type != VFS_FTBLK)
   {
      /* El supuesto nodo device no es device */
      return -1;
   }
   fs_driver = vfs_get_driver(fs_type);
   ASSERT_MSG(fs_driver!=NULL, "mount(): Device node not valid");
   if(fs_driver == NULL)
   {
      /* No existe un driver con el nombre dado */
      return -1;
   }
   /* Lleno los campos que corresponden del target inode */
   targetnode->fs_info.drv = fs_driver;
   targetnode->fs_info.device = devnode->fs_info.device;
   targetnode->n_info.is_mount_dir = true;
   /* Llamo a la funcion de bajo nivel */
   ret = fs_driver->driver_op->fs_mount(devnode, targetnode);
   ASSERT_MSG(ret==0, "mount(): Lower layer mount failed");
   if(ret)
   {
      /* Fallo el mount de bajo nivel */
      return -1;
   }
   return 0;
}

/*
 *  TODO: No puedo crear un subarbol, solo un archivo. Validar que en el path dado el unico elemento inexistente
 *  es el ultimo.
 */
extern int ciaaFS_mkdir(const char *dir_path, mode_t mode)
{
  vnode_t *dir_inode_p, *parent_inode_p;
  int               ret;

   char *tpath = (char *) dir_path;
   /*Creo un nodo pelado en el dir_path*/
   /*vfs_inode_reserve hereda los atributos de fs del padre*/
   ret = vfs_inode_reserve(tpath, &dir_inode_p);
   if(ret)
   {
      /* El directorio ya existe */
      return -1;
   }
   /*Lleno los campos del nodo*/
   dir_inode_p->n_info.type = VFS_FTDIR;
   /*Los atributos de fs ya fueron heredados mediante vfs_inode_reserve*/
   parent_inode_p = dir_inode_p->parent_node;
   ret = (dir_inode_p->fs_info).drv->driver_op->fs_create_node(parent_inode_p, dir_inode_p);
   if(ret)
   {
      /* TODO: No se pudo crear el nodo de la capa inferior. Manejar la situacion */
      return -1;
   }

   return 0;
}

extern int ciaaFS_rmdir(const char *dir_path)
{
   vnode_t *dir_inode_p, *parent_inode_p;
   char
   int               ret;

   char *tpath = (char *) dir_path;
   /*Creo un nodo pelado en el dir_path*/
   /*vfs_inode_reserve hereda los atributos de fs del padre*/
   if(dir_path->parent_node == NULL)
   {
      /* Cannot erase root */
      return -1;
   }

   auxpath = (char *) path;
   ret = vfs_inode_search(&auxpath, &target_inode);   /* Devuelve 0 si encuentra el nodo */
   /*Lleno los campos del nodo*/
   dir_inode_p->n_info.type = VFS_FTDIR;
   /*Los atributos de fs ya fueron heredados mediante vfs_inode_reserve*/
   parent_inode_p = dir_inode_p->parent_node;
   ret = (dir_inode_p->fs_info).drv->driver_op->fs_create_node(parent_inode_p, dir_inode_p);
   if(ret)
   {
      /* TODO: No se pudo crear el nodo de la capa inferior. Manejar la situacion */
      return -1;
   }

   return 0;
}

/*
 *  TODO: No puedo crear un subarbol, solo un archivo. Validar que en el path dado el unico elemento inexistente
 *  es el ultimo.
 */
extern int ciaaFS_open(const char *path, int flags) {

   vnode_t *target_inode, *parent_inode;
   struct file_desc *file;
   char *auxpath;
   int ret, fd;

   auxpath = (char *) path;
   ret = vfs_inode_search(&auxpath, &target_inode);   /* Devuelve 0 si encuentra el nodo */
   if (ret)   /* El nodo no existia, hay que crear uno nuevo si la opcion esta dada, sino error */
   {
      ciaaPOSIX_printf("open(): El nodo no existia, hay que crear uno nuevo\n");
      if (flags & VFS_O_CREAT)
      {
         ciaaPOSIX_printf("open(): Se dio orden de crearlo, lo creo\n");
         /* Creo un nodo pelado en el dir_path */
         /* vfs_inode_reserve hereda los atributos de fs del padre */
         auxpath= (char *)path;
         ret = vfs_inode_reserve(auxpath, &target_inode);
         ASSERT_MSG(target_inode!=NULL, "open(): vfs_inode_reserve() failed");
         if(target_inode==NULL)
         {
            /* Hubo un error al crear el nodo */
            return -1;
         }
         target_inode->n_info.type = VFS_FTREG;
         /* Llenar todos los campos como kcreat */
         /* Creo el archivo en la capa inferior */
         parent_inode = target_inode->parent_node;
         ret = target_inode->fs_info.drv->driver_op->fs_create_node(parent_inode, target_inode);
         ASSERT_MSG(ret>=0, "open(): fs_create_node() failed");
         if(ret)
         {
            /* No se pudo crear el nodo a bajo nivel. Manejar la situacion */
            return -1;
         }
      }
      else
      {
         /* El nodo no existia pero no se dio la orden de crearlo. Devuelvo error */
         return -1;
      }
   }
   else      /* El nodo ya existe, no hay que crearlo, solo abrir el archivo */
   {
      /* When used with O_CREAT, if the file already exists it is an error
       * and the open() will fail
       */
      if ((flags & VFS_O_EXCL) && (flags & VFS_O_CREAT))
      {
         return -1;
      }

      if(target_inode->n_info.type == VFS_FTDIR)
      {
         /* No puedo abrir un directorio o un nodo desconocido */
         return -1;
      }
   }

   file = file_desc_create(target_inode);
   ASSERT_MSG(file!=NULL, "open(): file_desc_create() failed");
   if(file == NULL)
   {
      return -1;
   }
   fd = file->index;
   ret = file->node->fs_info.drv->driver_op->file_open(file);
   ASSERT_MSG(ret>=0, "open(): file_open() failed");
   if(ret)
   {
      file_desc_destroy(file);
      return -1;
   }

   return fd;
}

extern int ciaaFS_close(int fd)
{
   struct file_desc *file;
   ssize_t ret;

   /* Obtener el descriptor de archivo a partir de fd */
   file = file_desc_get(fd);
   /* Validar el descriptor de archivo */
   ASSERT_MSG(file!=NULL, "read(): file_desc_get() failed");
   if(file==NULL)
   {
      /* Invalid file descriptor */
      return -1;
   }
   ret = file_desc_destroy(file);
   if(ret)
   {
      return -1;
   }
   return 0;
}

extern ssize_t ciaaFS_read(int fd, void *buf, size_t nbytes)
{
   struct file_desc *file;
   ssize_t ret;

   /* Obtener el descriptor de archivo a partir de fd */
   file = file_desc_get(fd);
   /* Validar el descriptor de archivo */
   ASSERT_MSG(file!=NULL, "read(): file_desc_get() failed");
   if(file==NULL)
   {
      /* Invalid file descriptor */
      return -1;
   }

   /* Verificar que el driver soporta lectura */
   /* No. Error */
   if(file->node->fs_info.drv->driver_op->file_read == NULL)
   {
      /* El archivo no soporta lectura */
      return 1;
   }
   /* Rutina de lectura del driver */
   ret = file->node->fs_info.drv->driver_op->file_read(file, buf, nbytes);
   ASSERT_MSG(ret==nbytes, "read(): file_read() failed");
   if(ret!=nbytes)
   {
      /* Setear ERROR para indicar que no se pudo leer todo */
      return ret;
   }
   return ret;
}

extern ssize_t ciaaFS_write(int fd, void *buf, size_t nbytes)
{
   struct file_desc *file;
   ssize_t ret;

   /* Obtener el descriptor de archivo a partir de fd */
   file = file_desc_get(fd);
   /* Validar el descriptor de archivo */
   ASSERT_MSG(file!=NULL, "read(): file_desc_get() failed");
   if(file==NULL)
   {
      /* Invalid file descriptor */
      return -1;
   }

   /* Verificar que el driver soporta escritura */
   /*No. Error*/
   if(file->node->fs_info.drv->driver_op->file_write == NULL)
   {
      /* El archivo no soporta escritura */
      return 1;
   }
   /* Rutina de escritura del driver */
   ret = file->node->fs_info.drv->driver_op->file_write(file, buf, nbytes);
   ASSERT_MSG(ret==nbytes, "write(): file_write() failed");
   if(ret!=nbytes)
   {
      /* Setear ERROR para indicar que no se pudo escribir todo */
      return ret;
   }
   return ret;
}

extern ssize_t ciaaFS_lseek(int fd, ssize_t offset, int whence)
{
   struct file_desc *file;
   ssize_t pos;

   /* Obtener el descriptor de archivo a partir de fd */
   file = file_desc_get(fd);
   /* Validar el descriptor de archivo */
   ASSERT_MSG(file!=NULL, "read(): file_desc_get() failed");
   if(file==NULL)
   {
      /* Invalid file descriptor */
      return -1;
   }

   pos=0;
   switch(whence)
   {
      case SEEK_END:
         pos = file->node->f_info.file_size + offset;
         break;
      case SEEK_CUR:
         pos = file->node->f_info.file_pointer + offset;
         break;
      default:
         pos = offset;
         break;
   }

   if ((pos >= 0) && (pos < file->node->f_info.file_size))
   {
      file->node->f_info.file_pointer = (uint32_t) pos;
   }
   return file->node->f_info.file_pointer;
}

extern int ciaaFS_unlink(const char *path)
{
   vnode_t *target_inode;
   struct filesystem_driver *driver;
   char *auxpath;
   int ret;

   auxpath = (char *) path;
   ret = vfs_inode_search(&auxpath, &target_inode);   /* Devuelve 0 si encuentra el nodo */
   if(ret)
   {
      /* File not found */
      return -1;
   } 
   if(target_inode->n_info.type != VFS_FTREG)
   {
      /* Not a regular file, can not unlink */
      return -1;
   }
   driver = target_inode->fs_info.drv;
   if(driver == NULL)
   {
      /*No driver. Fatal error*/
      return -1;
   }
   if(driver->driver_op->fs_delete_node == NULL)
   {
      /*The filesystem driver does not support this method*/
      return -1;
   }

   ret = driver->driver_op->fs_delete_node(target_inode->parent_node, target_inode);
   if(ret)
   {
      /* Filesystem op failed */
      return -1;
   } 
   /* Remove node from vfs */
   /*vfs_del_leaf(node);*/

	return 0;

}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
