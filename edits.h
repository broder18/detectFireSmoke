#ifndef EDITS_H
#define EDITS_H


struct Edits
{
public:
    Edits();
    //void set_smokeR(float* kt);
    float* get_smokeR();
    void set_smokeG(float* kt);
    float* get_smokeG();
    //void set_fireB(float* kt);
    float* get_fireB();
    void set_fireG(float* kt);
    float* get_fireG();
    void set_number(float* kt);
    float* get_number();
    void set_resolution(float* kt);
    float* get_resolution();
    void set_guard_interval_fire(float* kt);
    float* get_guard_interval_fire();
    void set_guard_interval_smoke(float* kt);
    float* get_guard_interval_smoke();
    void set_refresh_time(float* kt);
    float* get_refresh_time();
    void set_binar_factor(float* kt);
    float* get_binar_factor();
    void set_gaussian(float* kt);
    float* get_gaussian();

private:
    //К-ты для внутрикадровой обработки
    float smokeR = 0.07; //R для дыма
    float smokeG; //G для дыма
    float fireB = 0.18; //B для огня
    float fireG; //G для огня
    float number; //Кол-во кадров в выборке
    float resolution; //Размер окна сканирования
    float guard_interval_fire; //ЗИ для пламени
    float guard_interval_smoke; //ЗИ для дыма
    float refresh_time; //Время обновления порога в сек
    float binar_factor; //К-т бинаризации
    float gaussian;
};

#endif // EDITS_H
