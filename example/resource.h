#pragma once



void test_bro();
/*=====================================================
   This file is the public API, that should be used
   by the applications.
=====================================================*/

/*=====================================================
  Resource functions
=====================================================*/
void resource_init_default(VkInstance instance, VkDevice device,
                           TQueue graphic_queue,
                           VmaAllocator _Nonnull allocator,
                           T_Resource **out_res);

void resource_cleanup_frame(T_Resource *res, u32 frame_index);

/*=====================================================
   Buffer functions
=====================================================*/
ResourceIndex resource_create_buffer(T_Resource *_Nonnull res,
                                     TBufferCreateInfo *info,
                                     VkCommandBuffer _Nullable cmd);

AllocatedBuffer *resource_get_buffer(T_Resource *res, ResourceIndex res_base,
                                     u32 frame, bool is_swap);

TResult resource_update_reserve_space(T_Resource *res, VkCommandBuffer cmd,
                                      TReserveSpaceUpdateInfo info);

u32 resource_reserve_space(T_Resource *res, u32 len, ResourceIndex res_base,
                           u32 frame, bool swap);

void resource_write_buffer(T_Resource *_Nonnull res, TBufferWriteInfo info);

bool resource_has_reserved_len(T_Resource *res, ResourceIndex res_buffer,
                               u32 frame, bool swap);

void resource_cleanup_frame(T_Resource *res, u32 frame_index);

/*=====================================================
   Image Functions
=====================================================*/

ResourceIndex resource_create_image(T_Resource *res, VkCommandBuffer cmd,
                                    const TImageInfo *info);

void resource_resize_image(T_Resource *res, VkCommandBuffer cmd,
                           ResourceIndex image, VkExtent2D new_extent);

ResourceIndex resource_add_existing_image(T_Resource *res, VkCommandBuffer cmd,
                                          const TImageInfo *info,
                                          VkImage *existing_image);

void resource_replace_existing_index(T_Resource *res, VkCommandBuffer cmd,
                                     ResourceIndex index,
                                     const TImageInfo *info,
                                     VkImage existing_image);

AllocatedImage *resource_get_image(T_Resource *_Nonnull res,
                                   ResourceIndex image_index);
VkDescriptorSet resource_get_descriptor_set(const T_Resource *res);

/*=====================================================
   Other
=====================================================*/

VkPipelineLayout resource_create_pipeline_layout(const T_Resource *res);
