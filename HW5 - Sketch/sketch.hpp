#ifndef CSKECTH_H
#define CSKECTH_H

#include <iostream>
#include <algorithm>
#include <cstring>
#include <math.h>
#include "hasher.hpp"



class Sketch{

 protected:
  unsigned no_rows;
  unsigned no_cols;
  StrHash* hashes;
  long long int* table;
  double time;
  
  Sketch(double epsilon, double delta){
    no_rows = (log(1 / delta) / log(2));
    no_cols = (2 / epsilon);
    
    //prime column count
    bool found = false;
    
    while(!found){
      found = true;
      
      for(unsigned i = 2; i <= sqrt(no_cols); i++){
	if(no_cols % i == 0){
	  found = false;
	  no_cols++;
	}
      }
    }

    table = new long long int[this->no_rows * this->no_cols];
	memset(table, 0, this->no_rows*this->no_cols*sizeof(long long int));

    hashes = new StrHash[no_rows];
    for(unsigned i = 0; i < no_rows; i++){
      hashes[i].set_parameters(no_cols);
    }

    time = 0;
    
    std::cout << "A sketch with " << this->no_rows << " rows and " << this->no_cols << " columns is created" << std::endl;    
  }

 public:

  long long int get(const unsigned& i, const unsigned& j) const {
    return table[i * no_cols + j];
  }

  void add(const unsigned& i, const unsigned& j, const int& val){
    table[i * no_cols + j] += (long long int)val;
  }

  virtual void insert_to_row(const std::string& data, unsigned row_id) = 0;
  virtual void insert(const std::string& data) = 0;
  virtual long long int query(const std::string& data) const = 0;

  virtual std::string name(){
    return "Sketch";
  }

  void reset(){
    memset(table, 0, sizeof(unsigned)*no_rows*no_cols);
  }

  unsigned get_no_rows(){
    return no_rows;
  }

  unsigned get_no_cols(){
    return no_cols;
  }

  double getError(std::pair<std::string, int> freqs[]){
    double err = 0.0;

    for(int i = 0; i < 1000; i++){
      std::string curr = freqs[i].first;
      long long int actual = freqs[i].second;
      long long int guess = query(curr);

      double cerr = ((double)(abs(guess-actual)) / actual);
      err += cerr * cerr;
    }
    return sqrt(err / 1000);
  }
  
  void print() {
    for(unsigned i = 0; i < no_rows; i++) {
      for(unsigned j = 0; j < no_cols; j++) {
	std::cout << get(i, j) << " ";
      }
      std::cout << std::endl;
    }
  }
  

  void add_to_time(double partial){
    this->time += partial;
  }

  double get_time(){
    return this->time;
  }

  void reset_time(){
    this->time = 0;
  }
  
  virtual ~Sketch(){
    delete[] table;
    delete[] hashes;
  }
};

class FreqSketch: public Sketch{
protected:
  FreqSketch(double epsilon, double delta, int id):Sketch(epsilon,delta){
    //TO DO: IMPLEMENT DERIVED SKETCHES

  }

public:
  virtual void insert_to_row(const std::string& data, unsigned row_id){}
  virtual void insert(const std::string& data){}

  virtual long long int query(const std::string& data) const{return -1;} 

};

class CountSketch : public FreqSketch{
private:
	StrHash* g_hashes;
	int* results;

public:
	CountSketch(double epsilon, double delta, int id) : FreqSketch(epsilon,delta,id){
		
		results = new int[no_rows];
		g_hashes = new StrHash[no_rows];
		
		for(unsigned i = 0; i < no_rows; i++){
			g_hashes[i].set_parameters(no_cols);
		}
		    
		
	}

	virtual void insert_to_row(const std::string& data, unsigned row_id){
		unsigned col_id = hashes[row_id].hash(data);
		int contrib = 2 * (g_hashes[row_id].hash(data) % 2) - 1;
		add(row_id,col_id,contrib);
	}

	virtual void insert(const std::string& data){
		for(unsigned int i = 0; i < no_rows;i++) {
			insert_to_row(data,i);
		}
	}


	virtual long long int query(const std::string& data) const{
		long long int h_col = 0; 
		int coef = 0; 
	
		for (int i = 0; i <= no_rows;i++){
			results[i] = 0;
		}
		

		for(unsigned int i= 0; i < no_rows; i++){
			h_col = hashes[i].hash(data);
			coef = 2 * (g_hashes[i].hash(data)%2)-1;
			results[i] = get(i,h_col) * coef;
			
			if(results[i] < 0) {
				results[i] = 0;
			}

		}
		

		std::sort(results,results + no_rows);
		return results[no_rows / 2];
	} 

	virtual std::string name(){
		return "CS";
  }

	virtual ~CountSketch(){
		delete [] g_hashes;
		delete [] results;
	}

	
};


class CountMinSketch : public FreqSketch{
public:
	 CountMinSketch(double epsilon, double delta, int id) : FreqSketch(epsilon,delta,id){}

	virtual void insert_to_row(const std::string& data, unsigned row_id){
		
		unsigned col_id = hashes[row_id].hash(data);
		add(row_id,col_id,1);
		
	}

	virtual void insert(const std::string& data){
		for(unsigned int i = 0; i < no_rows;i++) {
			insert_to_row(data,i);
		}
	}

	virtual long long int query(const std::string& data) const{
		unsigned long long int r_freq, freq = std::numeric_limits<long long int>::max();
		unsigned h_col = 0;

		for(unsigned int i = 0; i < no_rows; i++) {
			h_col = hashes[i].hash(data);
			r_freq = get(i,h_col);
			if(r_freq < freq){
				freq = r_freq;
			}
		}
		return freq;
	}

	virtual std::string name(){
		return "CMS";
    }

	virtual ~CountMinSketch(){};

};


class CountMinMeanSketch : public CountMinSketch {
private:
	int * results;
public:
	CountMinMeanSketch(double epsilon, double delta, int id) : CountMinSketch(epsilon, delta, id){
		results = new int[no_rows];
	}

	virtual long long int query(const std::string& data) const{
		long long int no_stream, r_freq, noise = 0;
		unsigned h_col = 0;

		for (int i = 0; i <= no_rows;i++){
			results[i] = 0;
		}
		

		for(unsigned int i = 0; i < no_rows; i++){
			h_col = hashes[i].hash(data);
			r_freq = get(i,h_col);
			no_stream = 0;

			for(unsigned int j = 0; j < no_cols; j++){
				no_stream += get(i,j);
			}

			noise = ((no_stream - r_freq) / (no_cols-1));
			if(r_freq > noise) {
				results[i] = r_freq - noise;
			}
		}

		std::sort(results, results + no_rows);
		return results[no_rows/2];
	
	}

	virtual std::string name(){
		return "CMMS";
    }

	virtual ~CountMinMeanSketch(){
		delete [] results;
	};
};


#endif

// Berk Sezer