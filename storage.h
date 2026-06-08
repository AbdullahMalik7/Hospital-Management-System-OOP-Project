#pragma once
// Implementing the class here in header file to avoid the linker error
template <typename T>
class Storage {
    private:
    T data[100];
    int count;

    public:
    Storage(){
        count = 0;
    }

    int getSize() const {
        return count;
    }

    const T* getAll() const {
        return data;
    }

    bool add(const T& obj){
        if(count<100){
            data[count] = obj;
            count++;
            return true;
        }
        return false;
    }

    T* findByID(int id){
        for(int i = 0; i<count; i++){
            if(data[i].getID() == id){
                return &data[i];
            }
        }
        return nullptr;
    }

    const T* findByID(int id) const {
        for(int i = 0; i<count; i++){
            if(data[i].getID() == id){
                return &data[i];
            }
        }
        return nullptr;
    }

    T* getAt(int index){
        if(index >= 0 && index < count){
            return &data[index];
        }
        return nullptr;
    }


    bool removeByID(int id){
        for(int i = 0; i<count; i++){
            if(data[i].getID() == id){
                for(int j = i; j<count-1; j++){
                    data[j] = data[j+1];
                }
                count--;
                return true;
            }
        }
        return false;
    }

    bool removeAt(int index){
        if(index < 0 || index >= count){
            return false;
        }
        for(int j = index; j < count - 1; j++){
            data[j] = data[j + 1];
        }
        count--;
        return true;
    }


};