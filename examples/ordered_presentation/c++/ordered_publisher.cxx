/* ordered_publisher.cxx

   A publication of data of type ordered

   This file is derived from code automatically generated by the rtiddsgen 
   command:

   rtiddsgen -language C++ -example <arch> ordered.idl

   Example publication of type ordered automatically generated by 
   'rtiddsgen'. To test them follow these steps:

   (1) Compile this file and the example subscription.

   (2) Start the subscription with the command
       objs/<arch>/ordered_subscriber <domain_id> <sample_count>
                
   (3) Start the publication with the command
       objs/<arch>/ordered_publisher <domain_id> <sample_count>

   (4) [Optional] Specify the list of discovery initial peers and 
       multicast receive addresses via an environment variable or a file 
       (in the current working directory) called NDDS_DISCOVERY_PEERS. 
       
   You can run any number of publishers and subscribers programs, and can 
   add and remove them dynamically from the domain.

                                   
   Example:
        
       To run the example application on domain <domain_id>:
                          
       On Unix: 
       
       objs/<arch>/ordered_publisher <domain_id> o
       objs/<arch>/ordered_subscriber <domain_id> 
                            
       On Windows:
       
       objs\<arch>\ordered_publisher <domain_id>  
       objs\<arch>\ordered_subscriber <domain_id>    

       
modification history
------------ -------       
*/

#include <stdio.h>
#include <stdlib.h>
#ifdef RTI_VX653
#include <vThreadsData.h>
#endif
#include "ordered.h"
#include "orderedSupport.h"
#include "ndds/ndds_cpp.h"

/* Delete all entities */
static int publisher_shutdown(
    DDSDomainParticipant *participant)
{
    DDS_ReturnCode_t retcode;
    int status = 0;

    if (participant != NULL) {
        retcode = participant->delete_contained_entities();
        if (retcode != DDS_RETCODE_OK) {
            printf("delete_contained_entities error %d\n", retcode);
            status = -1;
        }

        retcode = DDSTheParticipantFactory->delete_participant(participant);
        if (retcode != DDS_RETCODE_OK) {
            printf("delete_participant error %d\n", retcode);
            status = -1;
        }
    }

    /* RTI Connext provides finalize_instance() method on
       domain participant factory for people who want to release memory used
       by the participant factory. Uncomment the following block of code for
       clean destruction of the singleton. */
/*
    retcode = DDSDomainParticipantFactory::finalize_instance();
    if (retcode != DDS_RETCODE_OK) {
        printf("finalize_instance error %d\n", retcode);
        status = -1;
    }
*/

    return status;
}

extern "C" int publisher_main(int domainId, int sample_count)
{
    DDSDomainParticipant *participant = NULL;
    DDSPublisher *publisher = NULL;
    DDSTopic *topic = NULL;
    DDSDataWriter *writer = NULL;
    orderedDataWriter * ordered_writer = NULL;
    DDS_ReturnCode_t retcode;
    const char *type_name = NULL;
    int count = 0;  
    DDS_Duration_t send_period = {1,0};

    /* To customize participant QoS, use 
       the configuration file USER_QOS_PROFILES.xml */
    participant = DDSTheParticipantFactory->create_participant(
        domainId, DDS_PARTICIPANT_QOS_DEFAULT, 
        NULL /* listener */, DDS_STATUS_MASK_NONE);
    if (participant == NULL) {
        printf("create_participant error\n");
        publisher_shutdown(participant);
        return -1;
    }

    /* To customize publisher QoS, use 
       the configuration file USER_QOS_PROFILES.xml */
    publisher = participant->create_publisher_with_profile(
        "ordered_Library", "ordered_Profile_subscriber_instance",
        NULL /* listener */, DDS_STATUS_MASK_NONE);
    if (publisher == NULL) {
        printf("create_publisher error\n");
        publisher_shutdown(participant);
        return -1;
    }

    /* If you want to change the Publisher's QoS programmatically rather than
     * using the XML file, you will need to add the following lines to your
     * code and comment out the create_publisher call above.
     *
     * In this case, we set the presentation publish mode ordered in the topic.
     */

    /* Get default publisher QoS to customize */
/*  DDS_PublisherQos publisher_qos;
    retcode = participant->get_default_publisher_qos(publisher_qos);
    if (retcode != DDS_RETCODE_OK) {
        printf("get_default_publisher_qos error\n");
        return -1;
    }

    publisher_qos.presentation.access_scope = DDS_TOPIC_PRESENTATION_QOS;
    publisher_qos.presentation.ordered_access = DDS_BOOLEAN_TRUE;

*/  /* To create publisher with default QoS, use DDS_PUBLISHER_QOS_DEFAULT
       instead of publisher_qos */
/*    publisher = participant->create_publisher(
        publisher_qos, NULL , DDS_STATUS_MASK_NONE);
    if (publisher == NULL) {
        printf("create_participant error\n");
        publisher_shutdown(participant);
        return -1;
    }
*/

    /* Register type before creating topic */
    type_name = orderedTypeSupport::get_type_name();
    retcode = orderedTypeSupport::register_type(
        participant, type_name);
    if (retcode != DDS_RETCODE_OK) {
        printf("register_type error %d\n", retcode);
        publisher_shutdown(participant);
        return -1;
    }

    /* To customize topic QoS, use 
       the configuration file USER_QOS_PROFILES.xml */
    topic = participant->create_topic(
        "Example ordered",
        type_name, DDS_TOPIC_QOS_DEFAULT, NULL /* listener */,
        DDS_STATUS_MASK_NONE);
    if (topic == NULL) {
        printf("create_topic error\n");
        publisher_shutdown(participant);
        return -1;
    }

    /* To customize data writer QoS, use 
       the configuration file USER_QOS_PROFILES.xml */
    writer = publisher->create_datawriter(
        topic, DDS_DATAWRITER_QOS_DEFAULT, NULL /* listener */,
        DDS_STATUS_MASK_NONE);
    if (writer == NULL) {
        printf("create_datawriter error\n");
        publisher_shutdown(participant);
        return -1;
    }
    ordered_writer = orderedDataWriter::narrow(writer);
    if (ordered_writer == NULL) {
        printf("DataWriter narrow error\n");
        publisher_shutdown(participant);
        return -1;
    }

    /* Start changes for Ordered_Presentation */
    /* Create two instances*/ 
    ordered *instance0 = NULL;
    ordered *instance1 = NULL;
    DDS_InstanceHandle_t handle0 = DDS_HANDLE_NIL;
    DDS_InstanceHandle_t handle1 = DDS_HANDLE_NIL;
    
    /* Create data sample for writing */

    instance0 = orderedTypeSupport::create_data();
    
    if (instance0 == NULL) {
        printf("orderedTypeSupport::create_data0 error\n");
        publisher_shutdown(participant);
        return -1;
    }

    instance1 = orderedTypeSupport::create_data();
    
    if (instance1 == NULL) {
        printf("orderedTypeSupport::create_data1 error\n");
        publisher_shutdown(participant);
        return -1;
    }

    /* For a data type that has a key, if the same instance is going to be
       written multiple times, initialize the key here
       and register the keyed instance prior to writing */
    instance0->id = 0;
    instance1->id = 1;

    handle0 = ordered_writer->register_instance(*instance0);
    handle1 = ordered_writer->register_instance(*instance1);

    /* Main loop */
    for (count=0; (sample_count == 0) || (count < sample_count); ++count) {

        instance0->value = count;
        instance1->value = count;


        printf("writing instance0, value->%d\n", instance0->value);
        retcode = ordered_writer->write(*instance0, handle0);
        if (retcode != DDS_RETCODE_OK) {
            printf("write error %d\n", retcode);
        }

        printf("writing instance1, value->%d\n", instance1->value);
        retcode = ordered_writer->write(*instance1, handle1);
        if (retcode != DDS_RETCODE_OK) {
            printf("write error %d\n", retcode);
        }
        NDDSUtility::sleep(send_period);
    }


    retcode = ordered_writer->unregister_instance(
        *instance0, handle0);
    if (retcode != DDS_RETCODE_OK) {
        printf("unregister instance0 error %d\n", retcode);
    }

    retcode = ordered_writer->unregister_instance(
        *instance1, handle1);
    if (retcode != DDS_RETCODE_OK) {
        printf("unregister instance1 error %d\n", retcode);
    }

    /* Delete data sample */
    retcode = orderedTypeSupport::delete_data(instance0);
    if (retcode != DDS_RETCODE_OK) {
        printf("orderedTypeSupport::delete_data0 error %d\n", retcode);
    }

    retcode = orderedTypeSupport::delete_data(instance1);
    if (retcode != DDS_RETCODE_OK) {
        printf("orderedTypeSupport::delete_data1 error %d\n", retcode);
    }

    /* Delete all entities */
    return publisher_shutdown(participant);
}

#if defined(RTI_WINCE)
int wmain(int argc, wchar_t** argv)
{
    int domainId = 0;
    int sample_count = 0; /* infinite loop */ 
    
    if (argc >= 2) {
        domainId = _wtoi(argv[1]);
    }
    if (argc >= 3) {
        sample_count = _wtoi(argv[2]);
    }

     /* Uncomment this to turn on additional logging
    NDDSConfigLogger::get_instance()->
        set_verbosity_by_category(NDDS_CONFIG_LOG_CATEGORY_API, 
                                  NDDS_CONFIG_LOG_VERBOSITY_STATUS_ALL);
    */
    
    return publisher_main(domainId, sample_count);
}
 
#elif !(defined(RTI_VXWORKS) && !defined(__RTP__)) && !defined(RTI_PSOS)
int main(int argc, char *argv[])
{
    int domainId = 0;
    int sample_count = 0; /* infinite loop */

    if (argc >= 2) {
        domainId = atoi(argv[1]);
    }
    if (argc >= 3) {
        sample_count = atoi(argv[2]);
    }

    /* Uncomment this to turn on additional logging
    NDDSConfigLogger::get_instance()->
        set_verbosity_by_category(NDDS_CONFIG_LOG_CATEGORY_API, 
                                  NDDS_CONFIG_LOG_VERBOSITY_STATUS_ALL);
    */
    
    return publisher_main(domainId, sample_count);
}
#endif

#ifdef RTI_VX653
const unsigned char* __ctype = *(__ctypePtrGet());

extern "C" void usrAppInit ()
{
#ifdef  USER_APPL_INIT
    USER_APPL_INIT;         /* for backwards compatibility */
#endif
    
    /* add application specific code here */
    taskSpawn("pub", RTI_OSAPI_THREAD_PRIORITY_NORMAL, 0x8, 0x150000, 
        (FUNCPTR)publisher_main, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
   
}
#endif

