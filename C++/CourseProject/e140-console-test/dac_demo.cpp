#include <conio.h>
#include <math.h>
#include <stdio.h>
#include <vector>
#include "Lusbapi.h"

/*================================================================================================*/
#define K_DAC (32767 / 5.0)
#define ADDR_DAC_PARS   0x0160
#define ADDR_DIAG       0x10C0
#define DAC_MODE_SET_CONST_ON_STOP  0x01
#define DAC_MODE_NO_CLEAR_BUFFER    0x02
#define DAC_MODE_LOOP               0x10
/*================================================================================================*/

/*================================================================================================*/
struct point
    {
    double x;
    double y;
    point(double x_, double y_) : x(x_), y(y_) { }
    };

point operator+(const point& p1, const point& p2)
    {
    return point(p1.x + p2.x, p1.y + p2.y);
    }

point operator-(const point& p1, const point& p2)
    {
    return point(p1.x - p2.x, p1.y - p2.y);
    }

point operator*(const point& p, double coeff)
    {
    return point(p.x * coeff, p.y * coeff);
    }

point operator*(double coeff, const point& p)
    {
    return point(p.x * coeff, p.y * coeff);
    }

struct dac_sample
    {
    SHORT ch0;
    SHORT ch1;
    dac_sample(double u0, double u1)
        : ch0(SHORT(u0 * K_DAC)), ch1(SHORT(u1 * K_DAC)) { }
    dac_sample(const point& pt)
        : ch0(SHORT(pt.x * K_DAC)), ch1(-SHORT(pt.y * K_DAC)) { }
    };

struct shapedef
    {
    const char* name;
    unsigned int num_points;
    point center;
    double scale;
    double interval;
    bool need_circle;
    const point* points;
    point scale_pt(const point& pt) const
        { return (pt - center) * scale; }
    };

typedef std::vector<dac_sample> dac_vec;

#pragma pack(1)
typedef struct
    {
    unsigned short RateDiv;       
    unsigned char SyncWithADC;    
    unsigned char RunMode;        
    unsigned int TotalSamples;    
    unsigned short PreloadSamples;
    short StopState[2];           
    unsigned short StartOffset;
    }
    t_DAC_ParamStruct;
#pragma pack()
/*================================================================================================*/

/*================================================================================================*/
const point ShapeLogo[] =
    {
    point(-3.25, 3.25), point( -1.85, -4.05 ),
    point( 0.90, -4.05 ), point( -0.10, 1.15 ),
    point( 3.30, 1.15 ), point( 2.90, 3.25 )
    };

const point ShapeRabbit[] =
    {
    point(166.00, 159.00), point(167.84, 138.00), point(167.84, 138.00),
    point(171.87, 100.32), point(184.17, 63.53), point(207.13, 33.00),
    point(216.20, 20.93), point(229.15, 9.37), point(243.00, 3.03),
    point(251.55,-0.89), point(258.96,-2.26), point(265.47, 6.02),
    point(271.21, 13.31), point(276.16, 35.32), point(278.35, 45.00),
    point(284.12, 70.53), point(288.28, 100.23), point(282.10, 126.00),
    point(278.62, 140.51), point(273.42, 149.72), point(268.01, 163.00),
    point(261.11, 179.95), point(251.21, 210.95), point(251.00, 229.00),
    point(250.87, 240.05), point(250.19, 246.74), point(255.49, 257.00),
    point(256.61, 259.17), point(258.90, 263.77), point(261.21, 264.65),
    point(263.46, 265.51), point(266.89, 263.85), point(269.00, 262.99),
    point(269.00, 262.99), point(287.00, 256.33), point(287.00, 256.33),
    point(303.82, 250.73), point(331.38, 245.03), point(349.00, 245.00),
    point(349.00, 245.00), point(373.00, 245.00), point(373.00, 245.00),
    point(382.06, 245.01), point(391.09, 246.42), point(400.00, 247.92),
    point(454.21, 257.06), point(503.06, 285.75), point(540.08, 326.00),
    point(558.08, 345.56), point(576.46, 369.37), point(584.49, 395.00),
    point(591.04, 415.87), point(591.03, 433.58), point(591.00, 455.00),
    point(590.98, 469.08), point(587.96, 483.12), point(586.00, 497.00),
    point(589.23, 493.47), point(591.34, 490.85), point(596.00, 489.13),
    point(606.01, 485.44), point(621.76, 488.17), point(619.79, 502.00),
    point(618.62, 510.29), point(609.46, 524.35), point(605.31, 532.00),
    point(593.80, 553.22), point(583.74, 575.16), point(565.17, 591.49),
    point(559.48, 596.49), point(554.69, 600.07), point(548.00, 603.69),
    point(548.00, 603.69), point(536.51, 609.70), point(536.51, 609.70),
    point(534.21, 611.91), point(531.84, 623.20), point(530.46, 627.00),
    point(526.98, 636.55), point(519.87, 646.02), point(511.00, 651.12),
    point(498.90, 658.08), point(494.65, 655.85), point(483.00, 657.29),
    point(483.00, 657.29), point(448.00, 663.45), point(448.00, 663.45),
    point(408.96, 671.30), point(384.66, 685.41), point(343.00, 686.88),
    point(336.69, 687.28), point(330.30, 687.49), point(324.00, 686.88),
    point(316.40, 685.78), point(305.76, 683.34), point(300.21, 677.79),
    point(289.38, 666.95), point(297.05, 648.37), point(306.09, 639.04),
    point(320.33, 624.34), point(338.43, 622.48), point(355.00, 614.00),
    point(355.00, 614.00), point(322.00, 617.00), point(322.00, 617.00),
    point(308.95, 617.02), point(304.04, 616.11), point(292.00, 611.00),
    point(292.00, 611.00), point(277.86, 643.00), point(277.86, 643.00),
    point(271.94, 660.06), point(273.90, 672.33), point(256.00, 683.07),
    point(242.81, 690.99), point(226.97, 692.17), point(212.00, 692.00),
    point(202.98, 691.89), point(188.97, 689.30), point(186.42, 679.00),
    point(183.43, 666.96), point(197.44, 649.26), point(204.00, 640.00),
    point(204.00, 640.00), point(198.00, 640.00), point(198.00, 640.00),
    point(193.79, 639.98), point(192.17, 639.89), point(188.00, 638.99),
    point(185.81, 638.51), point(182.97, 637.80), point(181.02, 636.72),
    point(167.19, 629.16), point(182.80, 613.89), point(188.89, 607.00),
    point(190.69, 604.97), point(191.80, 603.65), point(192.58, 601.00),
    point(196.17, 588.94), point(191.08, 567.58), point(184.57, 557.00),
    point(182.80, 554.12), point(180.44, 550.49), point(177.91, 548.27),
    point(177.91, 548.27), point(152.00, 534.20), point(152.00, 534.20),
    point(134.16, 523.52), point(115.59, 510.21), point(103.44, 493.00),
    point(85.83, 468.05), point(82.66, 448.69), point(83.00, 419.00),
    point(83.00, 419.00), point(85.69, 395.00), point(85.69, 395.00),
    point(85.11, 391.32), point(78.52, 382.38), point(76.26, 378.00),
    point(70.81, 367.45), point(67.14, 355.96), point(67.00, 344.00),
    point(66.89, 333.93), point(68.87, 324.41), point(72.42, 315.00),
    point(74.51, 309.46), point(80.12, 297.11), point(81.01, 292.00),
    point(81.57, 288.80), point(81.04, 285.27), point(81.01, 282.00),
    point(80.90, 271.39), point(80.91, 262.45), point(83.51, 252.00),
    point(84.81, 246.78), point(88.80, 240.28), point(87.46, 235.00),
    point(86.22, 230.12), point(80.11, 217.75), point(77.54, 213.00),
    point(70.14, 199.34), point(60.36, 184.00), point(50.54, 172.00),
    point(44.51, 164.64), point(37.56, 158.07), point(32.44, 150.00),
    point(20.82, 131.73), point(19.28, 114.40), point(15.20, 94.00),
    point(15.20, 94.00), point(1.42, 47.00), point(1.42, 47.00),
    point(0.19, 42.48), point(-1.62, 32.16), point(2.43, 28.74),
    point(6.19, 25.57), point(13.15, 28.52), point(17.00, 30.32),
    point(25.50, 34.30), point(41.89, 47.29), point(50.00, 53.27),
    point(84.20, 78.46), point(118.23, 108.52), point(139.14, 146.00),
    point(142.80, 152.56), point(148.51, 164.92), point(150.81, 172.00),
    point(151.85, 175.22), point(154.95, 186.64), point(156.70, 188.49),
    point(159.09, 191.02), point(162.79, 190.93), point(166.00, 191.00)
    };

const shapedef AllShapes[] =
    {
    { "logo", sizeof(ShapeLogo)/sizeof(ShapeLogo[0]), point(0, 0), 5.0/6.0, 0.05, true, ShapeLogo },
    { "rabbit", sizeof(ShapeRabbit)/sizeof(ShapeRabbit[0]), point(310, 350), 0.01, 0.01, false, ShapeRabbit }
    };

/*================================================================================================*/

/*================================================================================================*/
/*------------------------------------------------------------------------------------------------*/
static inline double Distance(const point& a, const point& b)
    {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
    }
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
static inline double Part(double start, double end, double pos)
    {
    return start + (end - start) * pos;
    }
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
void MakeShape(const shapedef& S, dac_vec& buf)
    {
    for (unsigned int i = 0; i < S.num_points; i++)
        {
        unsigned int j = i + 1;
        if (j >= S.num_points) j = 0;
        /* draw line from point i to point j */
        double len = Distance(S.points[i], S.points[j]) * S.scale;
        int steps = int(len / S.interval + 0.5);
        for (int k = 0; k < steps; k++)
            {
            buf.push_back(
                dac_sample(S.scale_pt(
                    S.points[i] + (S.points[j] - S.points[i]) * (double(k) / steps)
                    ))
            );
            }
        buf.push_back(dac_sample(S.scale_pt(S.points[j])));
        }

    if (S.need_circle)
        {
        point start = S.points[0];
        double radius = Distance(start, S.center);
        double len = 2 * M_PI * radius;
        int steps = int(len / S.interval + 0.5);
        /* compute starting angle */
        double phi = acos((start.x - S.center.x) / radius);
        if (start.y < S.center.y)
            phi = -phi;
        /* draw lines for each circle segment */
        for (int k = 0; k < steps; k++)
            {
            phi += 2 * M_PI / steps;
            buf.push_back(dac_sample(S.scale_pt(S.center + radius * point(cos(phi), sin(phi)))));
            }
        buf.push_back(dac_sample(S.scale_pt(S.points[0])));
        }
    }
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
void Run(ILE140* pModule, dac_vec& buf)
    {
    BOOL found = FALSE;
    char ModuleName[32];
    MODULE_DESCRIPTION_E140 ModuleDescription;
    t_DAC_ParamStruct dp;
    IO_REQUEST_LUSBAPI rq;

    for (int i = 0; !found && (i < 127); i++)
        {
        found = pModule->OpenLDevice(i);
        }
    if (!found)
        {
        puts("ERROR: E-140 not found.");
        return;
        }

    if (!pModule->GetModuleName(ModuleName))
        {
        puts("ERROR: Cannot read module name.");
        return;
        }
    if (strcmp(ModuleName, "E140"))
        {
        puts("ERROR: Module is not E-140.");
        return;
        }

    if (!pModule->GET_MODULE_DESCRIPTION(&ModuleDescription))
        {
        puts("ERROR: Cannot read module descriptor.");
        return;
        }

    printf("Connected to %s (serial number %s).\n",
        ModuleName, ModuleDescription.Module.SerialNumber);

    /* Создать конфигурацию ЦАП, загрузить параметры, записать данные */
    ZeroMemory(&dp, sizeof(dp));
    dp.RateDiv = 0;
    dp.SyncWithADC = 0;
    dp.RunMode = DAC_MODE_LOOP | DAC_MODE_SET_CONST_ON_STOP;
    dp.PreloadSamples = buf.size();
    dp.TotalSamples = 0;
    dp.StopState[0] = dp.StopState[1] = 0;
    if (!pModule->STOP_DAC())
        {
        puts("ERROR: Cannot stop DAC.");
        return;
        }
    if (!pModule->PutArray((BYTE*)&dp, sizeof(dp), ADDR_DAC_PARS))
        {
        puts("ERROR: Cannot set DAC parameters.");
        return;
        }
    DWORD write_size = (buf.size() + 127) & ~127;
    buf.reserve(write_size);
    rq.Buffer = (SHORT*)&buf[0];
    rq.NumberOfWordsToPass = write_size * 2;
    rq.Overlapped = NULL;
    rq.TimeOut = 2000;
    if (!pModule->WriteData(&rq))
        {
        puts("ERROR: Cannot load data into DAC.\n");
        return;
        }
    if (!pModule->START_DAC())
        {
        puts("ERROR: Cannot start DAC.");
        return;
        }
    
    puts("Press [Esc] to exit...");
    while (!kbhit() || (getch() != '\x1B'));

    if (!pModule->STOP_DAC())
        {
        puts("ERROR: Cannot stop DAC.");
        return;
        }
    }
/*------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------*/
int main(int argc, char* argv[])
    {
    ILE140* pModule;
    dac_vec dac_data;

    puts("E14-140-MD DAC Demo\n"
         "-------------------");
    DWORD dll_ver = GetDllVersion();
    if (dll_ver != CURRENT_VERSION_LUSBAPI)
        {
        printf("ERROR: Wrong version of lusbapi.dll (need %1u.%1u, got %1lu.%1lu).\n",
            CURRENT_VERSION_LUSBAPI >> 16, CURRENT_VERSION_LUSBAPI & 0xFFFF,
            dll_ver >> 16, dll_ver & 0xFFF);
        exit(0);
        }
   
    unsigned int shape_idx = 0;
    if (argc > 1)
        {
        unsigned int i;
        for (i = 0; i < sizeof(AllShapes)/sizeof(AllShapes[0]); i++)
            {
            if (!strcmp(argv[1], AllShapes[i].name))
                {
                shape_idx = i;
                break;
                }
            }
        if (i == sizeof(AllShapes)/sizeof(AllShapes[0]))
            {
            printf("There is no drawing named %s\n", argv[1]);
            exit(0);
            }
        }

    puts("This program displays a vector drawing on an X-Y oscilloscope.\n"
         "Connection: X to DAC1-AGND, Y to DAC2-AGND.\n"
         "List of supported drawings (selected by entering drawing name as command line argument):");
    for (unsigned int i = 0; i < sizeof(AllShapes)/sizeof(AllShapes[0]); i++)
        {
        printf(" %c %s\n", (i == shape_idx) ? '*' : ' ', AllShapes[i].name);
        }

    MakeShape(AllShapes[shape_idx], dac_data);

    printf("Prepared %d samples.\n", dac_data.size());
    if (dac_data.size() > 5120)
        {
        puts("ERROR: too much data for DAC.");
        exit(0);
        }

    pModule = static_cast<ILE140*>(CreateLInstance((char*)"e140"));
    if (!pModule)
        {
        puts("ERROR: Cannot create instance of ILE140.");
        exit(0);
        }

    Run(pModule, dac_data);
        
    pModule->ReleaseLInstance();
    puts("Program terminated.");
    }
/*------------------------------------------------------------------------------------------------*/
/*================================================================================================*/
