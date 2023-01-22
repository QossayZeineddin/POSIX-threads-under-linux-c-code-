#include "local.h"

int NUM_LINE_FACTORY_MAIN = 0;  // Number of Production Lines For factory A
int NUM_EML_MAIN = 0;           // Number of Technical Employees at Each Line
int NUM_LINE_FACTORY_B = 0;     // Number of Production Lines For factory B
int NUM_TRUCK_A = 0;            // Number of Trucks in factory A
int NUM_TRUCK_B = 0;            // Number of Trucks in factory C
int CA_TRK_CAPACITY = 0;        // Capacity of Each Truck A
int CB_TRK_CAPACITY = 0;        // Capacity of Each Truck B
int S_1_CAPACITY = 0;           // Capacity of storage area s 1
int S_2_CAPACITY = 0;           // Capacity of storage area s 2
int NUM_L1_EML = 0;             // Number of unloaded Employees
int NUM_L2_EML = 0;             // Number of carry the boxes Employees
int NUM_L3_EML = 0;             // Number of carry the boxes Employees
int CAP_BOX = 0;                //  Capacity of Each prodect Box
int TIME_R_MAIN_TECH_MIN = 0;
int TIME_R_MAIN_TECH_MAX = 0;
int TIME_R_STOR = 0;            // Time Range Taken By Storage Employees
int TIME_R_LOAD_MIN = 0;
int TIME_R_LOAD_MAX = 0;        // Time Range Taken By Loading Employees
int TIME_R_DRIV_MIN = 0;
int TIME_R_DRIV_MAX = 0;        // Time Range Taken By Truck Drivers Employees
int NUM_LOAD_DETERGENT = 0;
int LOAD_DETERGRNY_TUCK_CAP = 0;
int NUM_TRUCK_DET = 0;
int TIME_R_EXPORT = 0;          // Time Range Taken By Loading Employees
int NUM_DETER_PRODUCT = 0;
int SIMULATION_TIME = 0;
int A_material = 0;
int B_material = 0;

// Define Threads
pthread_mutex_t storage_s1_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t storage_s2_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t truck_load_deter_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t load_on_deter_truck_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t EMP_take_A_B_prodect = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t Truck_Mutex_A = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t Truck_Mutex_B = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t Truck_Mutex_Detergent = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t Load_Con = PTHREAD_COND_INITIALIZER;
pthread_cond_t truck_available_A = PTHREAD_COND_INITIALIZER;
pthread_cond_t truck_available_B = PTHREAD_COND_INITIALIZER;
pthread_cond_t no_truck_available = PTHREAD_COND_INITIALIZER;
pthread_cond_t detergent_box_available = PTHREAD_COND_INITIALIZER;
pthread_cond_t available_s2_storage = PTHREAD_COND_INITIALIZER;
pthread_cond_t cont_ordiring = PTHREAD_COND_INITIALIZER;
pthread_cond_t cont_loading_truck = PTHREAD_COND_INITIALIZER;
pthread_cond_t cont_loading_truck2 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cont_loading_truck3 = PTHREAD_COND_INITIALIZER;
pthread_cond_t line_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t s2_full = PTHREAD_COND_INITIALIZER;

pthread_t test;

int line_MAIN_position[10] ={0};

int num_of_availableA = 0;
int num_of_availableB = 0;
int storage_room_s1 = 0;
int storage_room_s2 = 0;
int loadedA_Boxes = 0;
int truckNotAvailable = 1;
int truck_id = 0;
int N = 0;
int takenBoxes = 0;
pthread_mutex_t line_MAIN_mutex[5];
int rawA = 0;
int rawB = 0;
int sold_detergent = 0;
int suspend_MAIN_work = 0;
int terminate_flag = 0;
int num_deter_prod = 0;
int num_of_detergent_in_box[5];

void L1_emp_job(void * );
void L2_emp_job(void * );
void L3_emp_job(void * );
void truck_A_job(struct truck_info * );
void truck_B_job(struct truck_info * );
void load_on_detergent_truks(void * );
void truck_detergent_job(void * );
void alarmHandler(); /* Forward declaration of alarm handler */
void technic_empMAIN_job(struct emp_info * );
int load_deterg_truck = 0;

int main(int argc, char * argv[])
{

    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<read the input file>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    signal(SIGALRM, alarmHandler); /* Install signal handler */

    FILE * fptr_input;
    char inp_file_values[52][40];
    // open the file if exists
    if ((fptr_input = fopen(INPUT_FILE, "r")) == NULL)
    {
        perror("INPUT_FILE");
        exit(-1);
    }

    // read the file
    for (int i = 0; i < 52; i++)
    {
        fscanf(fptr_input, "%s\n", inp_file_values[i]);
        //printf("%s  \n",inp_file_values[i]);
        // printf("inp_file_values[%d] =  %s\n", i, inp_file_values[i]);
    }

    red2();
    printf("\n\t\t\tReading input arguments from file was DONE\n");
    normals();
    fflush(stdout);

    NUM_LINE_FACTORY_MAIN = atoi(inp_file_values[1]);
    NUM_EML_MAIN = atoi(inp_file_values[3]);
    NUM_LINE_FACTORY_B = atoi(inp_file_values[5]);
    NUM_TRUCK_A = atoi(inp_file_values[7]);
    NUM_TRUCK_B = atoi(inp_file_values[9]);
    CA_TRK_CAPACITY = atoi(inp_file_values[11]);
    CB_TRK_CAPACITY = atoi(inp_file_values[13]);
    S_1_CAPACITY = atoi(inp_file_values[15]);
    S_2_CAPACITY = atoi(inp_file_values[17]);
    NUM_L1_EML = atoi(inp_file_values[19]);
    NUM_L2_EML = atoi(inp_file_values[21]);
    NUM_L3_EML = atoi(inp_file_values[23]);
    CAP_BOX = atoi(inp_file_values[25]);
    TIME_R_MAIN_TECH_MIN = atoi(inp_file_values[27]);
    TIME_R_MAIN_TECH_MAX = atoi(inp_file_values[29]);
    TIME_R_STOR = atoi(inp_file_values[31]);
    TIME_R_LOAD_MIN = atoi(inp_file_values[33]);
    TIME_R_LOAD_MAX = atoi(inp_file_values[35]);
    TIME_R_DRIV_MIN = atoi(inp_file_values[37]);
    TIME_R_DRIV_MAX = atoi(inp_file_values[39]);
    NUM_LOAD_DETERGENT = atoi(inp_file_values[41]);
    LOAD_DETERGRNY_TUCK_CAP = atoi(inp_file_values[43]);
    NUM_TRUCK_DET = atoi(inp_file_values[45]);
    TIME_R_EXPORT = atoi(inp_file_values[47]);
    NUM_DETER_PRODUCT = atoi(inp_file_values[49]);
    SIMULATION_TIME = atoi(inp_file_values[51]);
    pthread_t technic_emp_Main[NUM_LINE_FACTORY_MAIN][NUM_EML_MAIN];
    pthread_t truck_A_driver[NUM_TRUCK_A];
    pthread_t truck_B_driver[NUM_TRUCK_B];
    pthread_mutex_t truck_A_mutex[NUM_TRUCK_A];
    pthread_mutex_t truck_B_mutex[NUM_TRUCK_B];
    pthread_t storage_emp_L1[NUM_L1_EML];
    pthread_t storage_emp_L2[NUM_L2_EML];
    pthread_t storage_emp_L3[NUM_L3_EML];

    int line_MAIN_position[NUM_EML_MAIN];
    int trucks_B[NUM_TRUCK_B];
    pthread_t load_deter_emp[NUM_LOAD_DETERGENT];
    pthread_t load_deter_trucks[NUM_TRUCK_DET];
    //pthread_t truck_det_mutex[NUM_TRUCK_DET];
    load_deterg_truck = NUM_TRUCK_DET;

    //=====================================  Create technical employees for each line ===============================================

    for (int i = 0; i < NUM_LINE_FACTORY_MAIN; i++)
    {
        for (int j = 0; j < NUM_EML_MAIN; j++)
        {
            struct emp_info * info;
            info = malloc(sizeof(struct emp_info));
            ( * info).line_id = i;
            ( * info).emp_id = (10 * i) + j;
            char * b = "MAIN";
            strcpy(( * info).factory_name, b);
            if (pthread_create( & technic_emp_Main[i][j], NULL, (void * ) technic_empMAIN_job, info))
                exit(3);
        }
    }

    //===================================== Create  Lines , Truck , employee  Mutexes and Set initial Position ===================================
    for (int i = 0; i < NUM_LINE_FACTORY_MAIN; i++)
    {
        pthread_mutex_init( & truck_A_mutex[i], NULL);
    }

    for (int i = 0; i < NUM_TRUCK_B; i++)
    {
        pthread_mutex_init( & truck_B_mutex[i], NULL);
    }

    for (int i = 0; i < NUM_LINE_FACTORY_MAIN; i++)
    {
        pthread_mutex_init( & line_MAIN_mutex[i], NULL);
        line_MAIN_position[i] = 0;
    }

    //=====================================  Create Loading Employees of type L1 ===============================================================

    for (int i = 0; i < NUM_L1_EML; i++)
    {
        if (pthread_create( & storage_emp_L1[i], NULL, (void * ) L1_emp_job, (void * ) NULL) != 0)
        {
            exit(1);
        }
    }

    //=====================================  Create Loading Employees of type L2 ===============================================================

    for (int i = 0; i < NUM_L2_EML; i++)
    {
        if (pthread_create( & storage_emp_L2[i], NULL, (void * ) L2_emp_job, (void * ) NULL) != 0)
        {
            exit(1);
        }
    }

    //=====================================  Create Loading Employees of type L3 ===============================================================

    for (int i = 0; i < NUM_L3_EML; i++)
    {
        if (pthread_create( & storage_emp_L3[i], NULL, (void * ) L3_emp_job, (void * ) NULL) != 0)
        {
            exit(1);
        }
    }

    //============================================  Create Truck Drivers of factory A ===============================================

    struct truck_info * info;
    info = malloc(sizeof(struct truck_info));
    info -> num_of_available = NUM_TRUCK_A - 1;
    char * A = "A";
    strcpy(( * info).factory_name, A);
    for (int i = 0; i < NUM_TRUCK_A; i++)
    {
        info -> truck_id = i;
        if (pthread_create( & truck_A_driver[i], NULL, (void * ) truck_A_job, info) != 0)
            exit(0);

        sleep(1);
    }

    //=====================================  Create Loading truks of the detergent factory===============================================================
    for (int i = 0; i < NUM_LOAD_DETERGENT; i++)
    {
        if (pthread_create( & load_deter_emp[i], NULL, (void * ) load_on_detergent_truks, (void * ) NULL) != 0)
        {
            exit(1);
        }
    }

    //============================================  Create Truck Drivers of factory B ===============================================

    info = malloc(sizeof(struct truck_info));
    info -> num_of_available = NUM_TRUCK_B - 1;
    char * B = "B";
    strcpy(( * info).factory_name, B);
    for (int i = 0; i < NUM_TRUCK_B; i++)
    {
        info -> truck_id = i + NUM_TRUCK_A;
        if (pthread_create( & truck_B_driver[i], NULL, (void * ) truck_B_job, info) != 0)
            exit(0);

        sleep(1);
    }

    //============================================  Create Truck Drivers of detergent factory ===============================================

    for (int i = 0; i < NUM_TRUCK_DET; i++)
    {
        if (pthread_create( & load_deter_trucks[i], NULL, (void * ) truck_detergent_job, NULL) != 0)
            exit(0);
        sleep(1);
    }

    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Termination >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    alarm(SIMULATION_TIME * 1000);
    while (1)
    {
        if (terminate_flag == 1)
        {
            for (int i = 0; i < NUM_LINE_FACTORY_MAIN; i++)
            {
                for (int j = 0; j < NUM_EML_MAIN; j++)
                {
                    pthread_cancel(technic_emp_Main[i][j]);
                }
            }

            for (int i = 0; i < NUM_TRUCK_A; i++)
            {
                pthread_mutex_destroy( & truck_A_mutex[i]);
                pthread_cancel(truck_A_driver[i]);
            }

            for (int i = 0; i < NUM_TRUCK_B; i++)
            {
                pthread_mutex_destroy( & truck_B_mutex[i]);
                pthread_cancel(truck_B_driver[i]);
            }

            for (int i = 0; i < NUM_L1_EML; i++)
            {
                pthread_cancel(storage_emp_L1[i]);
            }

            for (int i = 0; i < NUM_L2_EML; i++)
            {
                pthread_cancel(storage_emp_L2[i]);
            }

            for (int i = 0; i < NUM_L3_EML; i++)
            {
                pthread_cancel(storage_emp_L3[i]);
            }


            for (int i = 0; i < NUM_TRUCK_B; i++)
            {
                pthread_mutex_destroy( & line_MAIN_mutex[i]);
                pthread_cancel(truck_B_driver[i]);
            }

            for (int i = 0; i < NUM_LINE_FACTORY_MAIN; i++)
            {
                pthread_mutex_destroy( & truck_A_mutex[i]);
            }

            for (int i = 0; i < NUM_TRUCK_B; i++)
            {
                pthread_mutex_destroy( & truck_B_mutex[i]);
            }

            for (int i = 0; i < NUM_LINE_FACTORY_MAIN; i++)
            {
                pthread_mutex_destroy( & line_MAIN_mutex[i]);
            }

            for(int i = 0 ; i < NUM_TRUCK_DET ; i++)
            {
                pthread_cancel(load_deter_trucks[i]);
            }

            for(int i = 0 ; i < NUM_LOAD_DETERGENT ; i++)
            {
                pthread_cancel(load_deter_emp[i]);
            }

            
            if (num_deter_prod == NUM_DETER_PRODUCT)
            {
                red2();
                printf("\n\n\n\t\t\t The factory has manufactured a number of detergent products that exceeded [%d]\n\n\n", num_deter_prod);
                normals();
                fflush(stdout);
                break;
            }
            else
            {
                red2();
                printf("\n\n\n\t\t\t The simulation has been running for more than [%d] minutes\n\n\n", SIMULATION_TIME);
                normals();
                fflush(stdout);
                break;
            }
            pthread_cancel(test);
            pthread_mutex_destroy( & storage_s2_mutex);
            pthread_mutex_destroy( & storage_s1_mutex);
            pthread_mutex_destroy( & truck_load_deter_mutex);
            pthread_mutex_destroy( & Truck_Mutex_A);
            pthread_mutex_destroy( & Truck_Mutex_B);
            pthread_mutex_destroy( & Truck_Mutex_Detergent);
            pthread_mutex_destroy( & EMP_take_A_B_prodect);
            pthread_mutex_destroy( & load_on_deter_truck_mutex);
        }
    }
    return 0;
}

// ========================================  Drivers of truck A job ==================================

void truck_A_job(struct truck_info * data)

{
    int truck = data -> truck_id;
    yellow();
    printf("\n\t\t\t [A] Trucks = [%d] ID = [%ld]", truck, pthread_self());
    normals();
    fflush(stdout);
    sleep(NUM_TRUCK_A + NUM_TRUCK_B);
    while (1)
    {
        sleep(1);
        pthread_mutex_lock( & Truck_Mutex_A);
        if (num_of_availableA >= NUM_TRUCK_A - 1)
        {
            red();
            printf("\n\t\t\t There is no available Trucks that belong to Factory [A]");
            normals();
            fflush(stdout);
            pthread_cond_wait( & truck_available_A, & Truck_Mutex_A);
            num_of_availableA = 0;
        }
        else
        {
            // printf("\n\t\t\t Number of available Trucks that belong to factory [A] = {%d} with [id] = %ld", num_of_availableA, pthread_self());
            //fflush(stdout);
            num_of_availableA++;
            pthread_cond_signal( & no_truck_available);
            sleep(1);
        }
        pthread_mutex_unlock( & Truck_Mutex_A);
    }
}

// ========================================  Drivers of truck B job ==================================

void truck_B_job(struct truck_info * data)
{
    int truck = data -> truck_id;
    yellow();
    printf("\n\t\t\t [B] Trucks = [%d] with ID = [%ld]", truck, pthread_self());
    normals();
    fflush(stdout);
    sleep(NUM_TRUCK_A);
    while (1)
    {
        sleep(1);
        if (rawA != 0)
        {
            if (rawB / rawA > 2)
            {
                printf("\n\t\t\t Stop ordering product {B}, Number of raw [A] = {%d}, while raw [B] = {%d}", rawA, rawB);
                fflush(stdout);
                pthread_cond_wait( & cont_ordiring, & Truck_Mutex_B);
            }
        }
        pthread_mutex_lock( & Truck_Mutex_B);
        if (num_of_availableB >= NUM_TRUCK_B - 1)
        {
            red();
            printf("\n\t\t\t There is no available Trucks that belong to Factory [B]");
            normals();
            //fflush(stdout);
            pthread_cond_wait( & truck_available_B, & Truck_Mutex_B);
            num_of_availableB = 0;
        }
        else
        {
            //printf("\n\t\t\t Number of available Trucks that belong to Factory [B] = {%d} id = %ld", num_of_availableB, pthread_self());
            //fflush(stdout);
            num_of_availableB++;
            pthread_cond_broadcast( & no_truck_available);
            sleep(1);
        }
        pthread_mutex_unlock( & Truck_Mutex_B);
    }
}

// ========================================  L1 employees job ==================================

void L1_emp_job(void * data)
{
    sleep(NUM_TRUCK_A + NUM_TRUCK_B + 1);
    while (1)
    {
        sleep(1);
        if (rawA != 0)
        {
            if (rawB / rawA < 1)
            {
                //printf("\n\t\t\t Resume ordering product {B}, Number of raw [A] = {%d}, while raw [B] = {%d}", rawA, rawB);
                pthread_cond_signal( & cont_ordiring);
            }
        }
        if (pthread_mutex_lock( & storage_s1_mutex) == 0)
        {
            printf("\n");
            if (storage_room_s1 >= S_1_CAPACITY)
            {
                green();
                printf("\n\t\t\t [S1] CAPACITY IS FULLLL, Number of raw [A] = {%d}, while raw [B] = {%d}", rawA, rawB);
                normals();
                fflush(stdout);
                // pthread_cond_signal( & Load_Con, & storage_s1_mutex);
            }
            if (storage_room_s1 > 3)
            {
                green();
                printf("\n\t\t\t L2 will load from [S1] ");
                normals();
                fflush(stdout);
                pthread_cond_signal( & Load_Con);
            }

            if (num_of_availableA == 0 && num_of_availableB == 0) //check if truck is available (unavailable then wait)
                pthread_cond_wait( & no_truck_available, & storage_s1_mutex);

            if (num_of_availableA != 0)
            {
                while (num_of_availableA > 0)
                {
                    num_of_availableA--;
                    storage_room_s1 += CA_TRK_CAPACITY;
                    rawA += CA_TRK_CAPACITY;
                    if (storage_room_s1 >= S_1_CAPACITY)
                    {
                        storage_room_s1 = S_1_CAPACITY;
                        rawA -= CA_TRK_CAPACITY;
                        yellow();
                        printf("\n\t\t\t [L1 %ld] Employee is now unloading Truck {A}, the storage (S1) capacity is now = [%d]", pthread_self(), storage_room_s1);
                        normals();
                        fflush(stdout);
                        break;
                    }
                    yellow();
                    printf("\n\t\t\t [L1 %ld] Employee is now unloading Truck {A}, the storage (S1) capacity is now = [%d]", pthread_self(), storage_room_s1);
                    normals();
                    fflush(stdout);
                }
                pthread_cond_broadcast( & truck_available_A);
            }

            if (num_of_availableB != 0)
            {
                while (num_of_availableB > 0)
                {
                    num_of_availableB--;
                    storage_room_s1 += CB_TRK_CAPACITY;
                    rawB += CB_TRK_CAPACITY;

                    if (storage_room_s1 >= S_1_CAPACITY)
                    {
                        storage_room_s1 = S_1_CAPACITY;
                        rawB -= CB_TRK_CAPACITY;
                        yellow();
                        printf("\n\t\t\t [L1  %ld] Employee is now unloading Truck {B}, the storage (S1) capacity is now = [%d]", pthread_self(), storage_room_s1);
                        normals();
                        fflush(stdout);
                        break;

                    }
                    yellow();
                    printf("\n\t\t\t [L1  %ld] Employee is now unloading Truck {B}, the storage (S1) capacity is now = [%d]", pthread_self(), storage_room_s1);
                    normals();
                    fflush(stdout);
                }
                pthread_cond_broadcast( & truck_available_B);
            }
            printf("\n");
            sleep(2);
            pthread_mutex_unlock( & storage_s1_mutex);
        }
        else
        {
            printf("UNLOAD EMPLOYEE: CAN'T LOCK \n");
            fflush(stdout);
        }
    }
}

// ========================================  L2 employees job ==================================

void L2_emp_job(void * data)
{
    while (1)
    {
        sleep(1);
        if (pthread_mutex_lock( & storage_s1_mutex) == 0)
        {
            pthread_cond_wait( & Load_Con, & storage_s1_mutex);
            srand(time(0));
            int rawbox = (int)(1 + (rand() % (storage_room_s1 + 1)));
            srand(time(0));
            int takenA = (int)(1 + (rand() % (rawbox + 1)));
            storage_room_s1 = storage_room_s1 - rawbox;
            A_material = A_material + takenA;
            B_material = B_material + (rawbox - takenA);
            yellow();
            printf("\n\t\t\t [L2 %ld] Employee is now loading %d pieces from [A] and %d pieces from [B] to the manufacturing lines", pthread_self(), A_material, B_material);
            normals();
            fflush(stdout);
            sleep(1);
            pthread_cond_signal( & line_cond);

        }
        pthread_mutex_unlock( & storage_s1_mutex);
    }
}

// ========================================  L3 employees job ==================================

void L3_emp_job(void * data)
{
    while (1)
    {
        sleep(1);
        if (pthread_mutex_lock( & storage_s2_mutex) == 0)
        {
            pthread_cond_wait( & detergent_box_available, & storage_s2_mutex);

            storage_room_s2 += 1;
            if (storage_room_s2 >= S_2_CAPACITY)
            {
                blue();
                printf("\n\t\t\t [S2] Storage is FULLLL!!");
                normals();
                fflush(stdout);
                pthread_cond_wait( & available_s2_storage, & storage_s2_mutex);
            }
            yellow();
            printf("\n\t\t\t [L3 %ld] Employee is now loading the Storage with boxes, Storage (S2) Capacity is now = [%d]\n", pthread_self(), storage_room_s2);
            normals();
            fflush(stdout);
            pthread_cond_signal( & cont_loading_truck3);
            sleep(TIME_R_STOR);
            printf("\n");
        }
        pthread_mutex_unlock( & storage_s2_mutex);
    }
}

// ========================================  Load to detergent trucks  ==================================

void load_on_detergent_truks(void * data)

{
    while (1)
    {
        sleep(1);

        if (pthread_mutex_lock( & truck_load_deter_mutex) == 0)
        {

            if (storage_room_s2 == 0)
            {
                pthread_cond_wait( & cont_loading_truck3, & truck_load_deter_mutex);
            }

            if (load_deterg_truck == 0)
            {
                pthread_cond_wait( & cont_loading_truck2, & truck_load_deter_mutex);
            }

            //sleep(1);
            ++takenBoxes;

            --storage_room_s2;
            if (storage_room_s2 + 1 >= S_2_CAPACITY)
            {
                sleep(1);
                pthread_cond_signal( & available_s2_storage);
                pthread_cond_broadcast( & s2_full);
            }
            blue();
            printf("\n\t\t\t [%ld] Employee is now loading the Detergent Truck with boxes, Boxes loaded  = [%d]\n", pthread_self(), takenBoxes);
            normals();
            fflush(stdout);

            if (takenBoxes == LOAD_DETERGRNY_TUCK_CAP)
            {
                takenBoxes = 0;
                --load_deterg_truck;
                pthread_cond_signal( & cont_loading_truck);
            }
        }
        pthread_mutex_unlock( & truck_load_deter_mutex);
    }
}

// ========================================  Detergent trucks job ==================================

void truck_detergent_job(void * data)
{
    while (1)
    {
        sleep(1);

        if (pthread_mutex_lock( & Truck_Mutex_Detergent) == 0)
        {
            pthread_cond_wait( & cont_loading_truck, & Truck_Mutex_Detergent);
            blue();
            printf("\n\t\t\t [Truck %ld] has left the detergent factory", pthread_self());
            normals();
            fflush(stdout);
            sleep(TIME_R_EXPORT);
            blue();
            printf("\n\t\t\t [Truck %ld] has returnd to the detergent factory\n", pthread_self());
            normals();
            fflush(stdout);
            ++load_deterg_truck;
            pthread_cond_signal( & cont_loading_truck2);
            printf("\n");
        }
        else
        {
            printf("TRUCK: CAN'T LOCK \n");
            fflush(stdout);
        }
        pthread_mutex_unlock( & Truck_Mutex_Detergent);
    }
}

// ========================================  	Technical employees job ==================================

void technic_empMAIN_job(struct emp_info * data)
{
    // Receive Data Encapsulated Inside a Struct, Extract Data and Free the Struct
    int line = data -> line_id;
    int id = data -> emp_id;
    char factory_name[4];
    strcpy(factory_name, data -> factory_name);
    //free (data);
    green();
    printf("\n\t\t\t Technical employees  line is = %d ,, id is = %d  ,, factory_name is %s  created, ID = [%ld]", line, id, factory_name, pthread_self());
    normals();
    fflush(stdout);
    id = id % 10;
    srand(time(0));
    int processing = 0;
    sleep(5);

    if (strcmp(factory_name, "MAIN") == 0)
    {
        while (1)
        {
            sleep(1);
            if (id == 0 && id == line_MAIN_position[line])
            {
                //check if the product don't go in infiniti
                if (processing < 10)
                {
                    pthread_mutex_lock( & EMP_take_A_B_prodect);
                    pthread_cond_wait( & line_cond, & EMP_take_A_B_prodect);

                    if (B_material >= 1 && A_material >= 2)
                    {
                        B_material--;
                        A_material = A_material - 2; // take tow of A and one of B
                        storage_room_s1 = storage_room_s1 - 3;
                        cyan();
                        printf("\n\t\t\t Empolyee id[%d] in line [%d] takes two products from [A] and one product from [B] and start working, raw A is %d, raw B is %d \n", id, line, A_material, B_material);
                        normals();
                        fflush(stdout);
                        //pthread_cond_broadcast(& Load_Con);
                        processing++; // do some prossing in the prodect
                        //printf("\n\n\ns1 %d ,, A %d  ,, B %d     %d\n\n",storage_room_s1,A_prodect_in_s_1,B_prodect_in_s_1 , id);
                        sleep((rand() % (TIME_R_MAIN_TECH_MAX - TIME_R_MAIN_TECH_MIN + 1)) + TIME_R_MAIN_TECH_MIN);
                        line_MAIN_position[line]++; // go to the next employee
                        ///pthread_mutex_unlock(&line_MAIN_mutex[line]);
                    }
                    pthread_mutex_unlock( & EMP_take_A_B_prodect);
                }
            }
            else if (id == 1 && id == line_MAIN_position[line])
            {
                if (processing < 10)
                {
                    pthread_mutex_lock( & line_MAIN_mutex[line]);
                    processing++; // do some prossing in the prodect
                    cyan();
                    printf("\n\t\t\t Employee  id [%d] in line[%d] and position[%d]  done his job next EMP start working", id, line, line_MAIN_position[line]);
                    normals();
                    fflush(stdout);
                    line_MAIN_position[line]++; // go to the next employee
                    sleep((rand() % (TIME_R_MAIN_TECH_MAX - TIME_R_MAIN_TECH_MIN + 1)) + TIME_R_MAIN_TECH_MIN);
                    pthread_mutex_unlock( & line_MAIN_mutex[line]);
                }
            }
            else if (id == 2 && id == line_MAIN_position[line])
            {
                if (processing < 10)
                {
                    pthread_mutex_lock( & line_MAIN_mutex[line]);
                    processing++; // do some prossing in the prodect
                    cyan();
                    printf("\n\t\t\t Employee  id [%d] in line [%d] finished his job  and next employee starts working", id, line);
                    normals();
                    fflush(stdout);
                    line_MAIN_position[line]++; // go to the next employee
                    sleep((rand() % (TIME_R_MAIN_TECH_MAX - TIME_R_MAIN_TECH_MIN + 1)) + TIME_R_MAIN_TECH_MIN);
                    pthread_mutex_unlock( & line_MAIN_mutex[line]);
                }
            }
            else if (id == 3 && id == line_MAIN_position[line])
            {
                if (processing < 10)
                {
                    pthread_mutex_lock( & line_MAIN_mutex[line]);
                    processing++; // do some prossing in the prodect
                    cyan();
                    printf("\n\t\t\t Employee  id [%d] in line [%d] finished his job  and next employee starts working", id, line);
                    normals();
                    fflush(stdout);
                    line_MAIN_position[line]++; // go to the next employee
                    sleep((rand() % (TIME_R_MAIN_TECH_MAX - TIME_R_MAIN_TECH_MIN + 1)) + TIME_R_MAIN_TECH_MIN);
                    pthread_mutex_unlock( & line_MAIN_mutex[line]);
                }
            }
            else if (id == 4 && id == line_MAIN_position[line])
            {
                if (processing < 10)
                {
                    pthread_mutex_lock( & line_MAIN_mutex[line]);
                    processing++; // do some prossing in the prodect
                    cyan();
                    printf("\n\t\t\t Employee  id [%d] in line [%d] finished his job  and next employee starts working", id, line);
                    normals();
                    fflush(stdout);
                    line_MAIN_position[line]++; // go to the next employee
                    sleep((rand() % (TIME_R_MAIN_TECH_MAX - TIME_R_MAIN_TECH_MIN + 1)) + TIME_R_MAIN_TECH_MIN);
                    pthread_mutex_unlock( & line_MAIN_mutex[line]);
                }
            }
            else if (id == 5 && id == line_MAIN_position[line])
            {
                if (processing < 10)
                {
                    pthread_mutex_lock( & line_MAIN_mutex[line]);
                    processing++; // do some prossing in the prodect
                    cyan();
                    printf("\n\t\t\t Employee  id [%d] in line [%d] finished his job  and next employee starts working", id, line);
                    normals();
                    fflush(stdout);
                    line_MAIN_position[line]++; // go to the next employee
                    sleep((rand() % (TIME_R_MAIN_TECH_MAX - TIME_R_MAIN_TECH_MIN + 1)) + TIME_R_MAIN_TECH_MIN);
                    pthread_mutex_unlock( & line_MAIN_mutex[line]);
                }
            }
            else if (id == 6 && id == line_MAIN_position[line])
            {
                if (processing < 10)
                {
                    pthread_mutex_lock( & line_MAIN_mutex[line]);
                    processing++; // do some prossing in the prodect
                    cyan();
                    printf("\n\t\t\t Employee  id [%d] in line [%d] finished his job  and next employee starts working", id, line);
                    normals();
                    fflush(stdout);
                    line_MAIN_position[line]++; // go to the next employee
                    sleep((rand() % (TIME_R_MAIN_TECH_MAX - TIME_R_MAIN_TECH_MIN + 1)) + TIME_R_MAIN_TECH_MIN);
                    pthread_mutex_unlock( & line_MAIN_mutex[line]);
                }
            }
            else if (id == 7 && id == line_MAIN_position[line])
            {
                if (processing < 10)
                {
                    pthread_mutex_lock( & line_MAIN_mutex[line]);
                    processing++; // do some prossing in the prodect
                    cyan();
                    printf("\n\t\t\t Employee  id [%d] in line [%d] finished his job  and next employee starts working", id, line);
                    normals();
                    fflush(stdout);
                    line_MAIN_position[line]++; // go to the next employee
                    sleep((rand() % (TIME_R_MAIN_TECH_MAX - TIME_R_MAIN_TECH_MIN + 1)) + TIME_R_MAIN_TECH_MIN);
                    pthread_mutex_unlock( & line_MAIN_mutex[line]);
                }
            }
            else if (id == 8 && id == line_MAIN_position[line])
            {
                if (processing < 10)
                {
                    pthread_mutex_lock( & line_MAIN_mutex[line]);
                    processing++; // do some prossing in the prodect
                    cyan();
                    printf("\n\t\t\t Employee  id [%d] in line [%d] finished his job  and next employee starts working", id, line);
                    normals();
                    fflush(stdout);
                    line_MAIN_position[line]++; // go to the next employee
                    sleep((rand() % (TIME_R_MAIN_TECH_MAX - TIME_R_MAIN_TECH_MIN + 1)) + TIME_R_MAIN_TECH_MIN);
                    pthread_mutex_unlock( & line_MAIN_mutex[line]);
                }
            }
            else if (id == 9 && id == line_MAIN_position[line])
            {
                printf("\n");
                if (processing < 10)
                {
                    pthread_mutex_lock( & line_MAIN_mutex[line]);
                    processing = 0;
                    line_MAIN_position[line] = 0; // end job and get box
                    Purple();
                    printf("\n\t\t\t Employee  id [%d] in line [%d] finished his job and the detergent manufacturing process is over", id, line);
                    normals();
                    fflush(stdout);
                    if (line == 0)
                    {
                        num_of_detergent_in_box[0]++;
                        green();
                        printf("\n\t\t\t NUMBER OF DETERGENT IN BOX of line %d is %d", line, num_of_detergent_in_box[0]);
                        normals();
                        fflush(stdout);
                        if (num_of_detergent_in_box[0] >= CAP_BOX)
                        {
                            num_deter_prod++;
                            red();
                            printf("\n\t\t\t Capacity of box in line %d is full and it's %d, Number of ready boxes is %d", line, CAP_BOX, num_deter_prod);
                            normals();
                            fflush(stdout);
                            pthread_cond_signal( & detergent_box_available);
                            num_of_detergent_in_box[0] = 0;
                        }
                    }
                    else if (line == 1)
                    {
                        num_of_detergent_in_box[1]++;
                        green();
                        printf("\n\t\t\t NUMBER OF DETERGENT IN BOX of line %d is %d", line, num_of_detergent_in_box[1]);
                        normals();
                        fflush(stdout);
                        if (num_of_detergent_in_box[1] >= CAP_BOX)
                        {
                            num_deter_prod++;
                            red();
                            printf("\n\t\t\t Capacity of box in line %d is full and it's %d, Number of ready boxes is %d", line, CAP_BOX, num_deter_prod);
                            normals();
                            fflush(stdout);
                            pthread_cond_signal( & detergent_box_available);
                            num_of_detergent_in_box[1] = 0;
                        }
                    }
                    else if (line == 2)
                    {
                        num_of_detergent_in_box[2]++;
                        green();
                        printf("\n\t\t\t NUMBER OF DETERGENT IN BOX of line %d is %d", line, num_of_detergent_in_box[2]);
                        normals();
                        fflush(stdout);
                        if (num_of_detergent_in_box[2] >= CAP_BOX)
                        {
                            num_deter_prod++;
                            red();
                            printf("\n\t\t\t Capacity of box in line %d is full and it's %d, Number of ready boxes is %d", line, CAP_BOX, num_deter_prod);
                            normals();
                            fflush(stdout);
                            pthread_cond_signal( & detergent_box_available);
                            num_of_detergent_in_box[2] = 0;
                        }
                    }
                    else if (line == 3)
                    {
                        num_of_detergent_in_box[3]++;
                        green();
                        printf("\n\t\t\t NUMBER OF DETERGENT IN BOX of line %d is %d", line, num_of_detergent_in_box[3]);
                        normals();
                        fflush(stdout);
                        if (num_of_detergent_in_box[3] >= CAP_BOX)
                        {
                            num_deter_prod++;
                            red();
                            printf("\n\t\t\t Capacity of box in line %d is full and it's %d, Number of ready boxes is %d", line, CAP_BOX, num_deter_prod);
                            normals();
                            fflush(stdout);
                            pthread_cond_signal( & detergent_box_available);
                            num_of_detergent_in_box[3] = 0;
                        }

                    }
                    else if (line == 4)
                    {
                        num_of_detergent_in_box[4]++;
                        green();
                        printf("\n\t\t\t NUMBER OF DETERGENT IN BOX of line %d is %d", line, num_of_detergent_in_box[4]);
                        normals();
                        fflush(stdout);
                        if (num_of_detergent_in_box[4] >= CAP_BOX)
                        {
                            num_deter_prod++;
                            red();
                            printf("\n\t\t\t Capacity of box in line %d is full and it's %d, Number of ready boxes is %d", line, CAP_BOX, num_deter_prod);
                            normals();
                            fflush(stdout);
                            pthread_cond_signal( & detergent_box_available);
                            num_of_detergent_in_box[4] = 0;
                        }
                    }
                    blue();
                    printf("\n\t\t\t NUMBER OF DETERGENT IN boxes #1[%d], #2[%d] , #3[%d] , #4[%d] , #2[%d]\n", num_of_detergent_in_box[0], num_of_detergent_in_box[1], num_of_detergent_in_box[2], num_of_detergent_in_box[3], num_of_detergent_in_box[4]);
                    normals();
                    fflush(stdout);
                    if (num_deter_prod == NUM_DETER_PRODUCT)
                    {
                        terminate_flag = 1;
                    }
                    sleep((rand() % (TIME_R_MAIN_TECH_MAX - TIME_R_MAIN_TECH_MIN + 1)) + TIME_R_MAIN_TECH_MIN);
                    pthread_mutex_unlock( & line_MAIN_mutex[line]);
                    // loaded the prodect in the
                }
            }
        }
    }
}

// Creates a Random Integer in a Specefic Range

void alarmHandler()
{
    printf("\n\t\t\t ALARM");
    terminate_flag = 1;
}
